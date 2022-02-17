#include "i2cHandler.h"

void scanI2C()
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println(" ");
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found.\n");
    else
        Serial.println("Done.\n");
}

void initI2C()
{
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.setClock(100000);
}

void wakeI2C(uint8_t address)
{
    uint8_t command = REGMAP_ORIGIN;
    uint8_t data[2] = {0};
    // 2 Dummy Writes to Wake up the MBR3 device
    writeI2C(address, command, data, 1);
    writeI2C(address, command, data, 1);
}

bool writeI2C(uint16_t address, uint8_t command, uint8_t *data, uint8_t dataLen)
{
    byte err;
    uint8_t dataPos = 0;
    uint8_t writtenBytes = 1;
    Wire.beginTransmission(address);
    writtenBytes = writtenBytes + Wire.write(command);
    for (dataPos; dataPos < dataLen; dataPos++)
    {
        writtenBytes = writtenBytes + Wire.write(byte(data[dataPos]));
    }
    err = Wire.endTransmission();
    delay(70);
    if (err > 0)
    {
        Serial.printf("writeI2C Error Code: 0x0%d\n", err);
        return false;
    }
    else
    {
        Serial.printf("Written Bytes: %d\n", writtenBytes);
        return true;
    }
}

bool readI2C(uint16_t address, uint8_t command, uint8_t *data, uint8_t dataLen)
{
    if (!writeI2C(SLAVE_ADDR, command, 0, 0))
    {
        return false;
    }
    Wire.requestFrom(address, dataLen);
    delay(10);
    if (Wire.available())
    {
        for (uint8_t dataPos = 0; dataPos < dataLen; dataPos++)
        {
            data[dataPos] = Wire.read();
        }
        return true;
    }
    else
        return false;
}

void setConfig(uint8_t *config, uint8_t debounce, uint8_t sensitivity)
{
    uint16_t temp16;
    // change Debounce
    config[SENSOR_DEBOUNCE] = debounce;
    if (global.is3108)
    {
        // change sensitivity 1
        config[BASE_THRESHOLD1] = sensitivity;
        // change sensitivity 2
        config[FINGER_THRESHOLD2] = sensitivity;
        // change sensitivity 3
        config[FINGER_THRESHOLD3] = sensitivity;
        // change sensitivity 4
        config[FINGER_THRESHOLD4] = sensitivity;
        // change sensitivity 5
        config[FINGER_THRESHOLD5] = sensitivity;
        // change sensitivity 6
        config[FINGER_THRESHOLD6] = sensitivity;
    }
    else
    {
        // change sensitivity 1
        config[FINGER_THRESHOLD6] = sensitivity;
        // change sensitivity 2
        config[FINGER_THRESHOLD7] = sensitivity;
        // change sensitivity 3
        config[FINGER_THRESHOLD8] = sensitivity;
        // change sensitivity 4
        config[FINGER_THRESHOLD10] = sensitivity;
        // change sensitivity 5
        config[FINGER_THRESHOLD11] = sensitivity;
        // change sensitivity 6
        config[FINGER_THRESHOLD12] = sensitivity;
    }
    // calculate CRC
    temp16 = CY8CMBR3xxx_CalculateCrc(config);
    // Split 16-bit integer to two byte spaces
    config[126] = temp16 & 0xFF;
    config[127] = (temp16 >> 8);
}

void configureI2C()
{
    uint8_t command, i;
    uint8_t data[2] = {0};
    uint8_t config[128] = {0}, retry = 3;
    static const uint8_t DebounceTable[10] = {1, 2, 3, 4, 5, 6, 8, 10, 12, 15};
    static const uint8_t SensitivityTable[6] = {45, 60, 80, 100, 130, 160};
    while (retry--)
    {
        wakeI2C(SLAVE_ADDR);
        // read device id
        command = DEVICE_ID;
        if (!readI2C(SLAVE_ADDR, command, data, 2))
        {
            Serial.println("Failed reading from I2C.");
            continue; // try again
        }
        else
        {
            uint16_t device_id = data[1] * 256 + data[0];
            // uint16_t device_id = DEVICE_ID_3108;
            if (device_id == DEVICE_ID_3108)
            {
                global.is3108 = 1;
                global.test3Touch = true;
                for (i = 0; i < 128; i++)
                    config[i] = CY8CMBR3108_LQXI_configuration[i];
            }
            else if (device_id == DEVICE_ID_3116)
            {
                // is3108 = 0;
                global.test3Touch = true;
                for (i = 0; i < 128; i++)
                    config[i] = CY8CMBR3116_LQXI_configuration[i];
            }
            else
            {
                continue;
                global.test3Touch = false;
            }
        }
        // send the configuration
        setConfig(config, DebounceTable[global.debounce], SensitivityTable[global.sensitivity]);
        command = REGMAP_ORIGIN;
        if (!writeI2C(SLAVE_ADDR, command, config, 128))
        {
            Serial.println("Failed to update config.");
            continue; // try again
        }
        // save
        command = CTRL_CMD;
        data[0] = SAVE_CHECK_CRC;
        if (!writeI2C(SLAVE_ADDR, command, data, 1))
        {
            Serial.println("Failed to calculate checksum");
            continue; // try again
        }
        // wait for 220ms
        // read error of command 0-no error, 254 - crc error
        command = CTRL_CMD_ERROR;
        if (!readI2C(SLAVE_ADDR, command, data, 1))
        {
            Serial.println("Failed to verify checksum.");
            continue; // try again
        }
        if (data[0] != COMMAND_SUCCESS)
        {
            Serial.println(CY8CMBR3xxx_CalculateCrc(config));
            Serial.println("Checksum Error.");
            continue; // try again
        }
        // reset
        command = CTRL_CMD;
        data[0] = SW_RESET;
        if (!writeI2C(SLAVE_ADDR, command, data, 1))
        {
            Serial.println("Reset failed.");
            continue; // try again
        }
        // success
        Serial.printf("CY8MBR Configuration succeeded. Try: %d\n", (3 - retry));
        break;
    }
}

void testCapacitanceI2C()
{
    uint8_t i;
    static uint8_t command;
    bool readAll = false;
    uint8_t data[2] = {0}, retry = 3;
    wakeI2C(SLAVE_ADDR);

    // clear FSS_EN (single sensor detection, we want multi sensor detection)
    command = FSS_EN;
    data[0] = 0;
    data[1] = 0;
    //			do{
    writeI2C(SLAVE_ADDR, command, data, 2);
    delay(160);
}

void getStatusI2C()
{
    uint8_t buttons[2] = {0};
    uint8_t prox[2] = {0}, data[2];
    uint8_t command, retry = 3;

    wakeI2C(SLAVE_ADDR);
    while (retry--)
    {
        // read proximity status
        command = PROX_STAT;
        if (!readI2C(SLAVE_ADDR, command, prox, 2))
            continue; // retry
        proxStat = prox[1] > 0;
        // check if to clear latch
        if ((prox[0] == 0) && (prox[1] > 0))
        {
            // save
            command = CTRL_CMD;
            data[0] = CLEAR_LATCHED_STATUS;
            if (!writeI2C(SLAVE_ADDR, command, data, 1))
                continue; // try again
        }
        // read buttons status
        command = BUTTON_STATUS;
        if (readI2C(SLAVE_ADDR, command, buttons, 1))
        {
            switch (buttons[0])
            {
            case 2:
                Serial.println("TEMPERATURE DOWN");
                break;
            case 4:
                Serial.println("POWER TOGGLE");
                break;
            case 8:
                Serial.println("TEMPERATURE UP");
                break;
            case 33:
                Serial.println("MODE");
                break;
            case 64:
                Serial.println("VENT");
                break;
            default:
                Serial.println("IDLE");
                break;
            }
        }
        else
            continue;
        break;
    }
}
