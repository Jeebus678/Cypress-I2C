#ifndef _I2CHANDLER_H_
#define _I2CHANDLER_H_

#include <Wire.h>
#include <Arduino.h>
#include "buttonVars.h"
#include <MasterTouch1p1.h>
#include <MasterTouch1p1_16.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <CY8CMBR3xxx_CRC.h>

#ifdef __cplusplus
}
#endif

#define SDA_PIN 21
#define SCL_PIN 22

struct i2c_data
{
    int debounce = 0;
    int sensitivity = 3;
    int is3108 = 1;
    bool test3Touch = true;
};

extern const unsigned char CY8CMBR3108_LQXI_configuration[128];
static i2c_data global;
static uint8_t readFlag = 0, proxStat = 0, reconfigureFlag = 0;
static uint16_t cyDly = 0, backLightPlusDly = 0;


void initI2C();
void scanI2C(); 
void wakeI2C(uint8_t address);
bool writeI2C(uint16_t address, uint8_t command, uint8_t *data, uint8_t dataLen);
bool readI2C(uint16_t address, uint8_t command, uint8_t *data, uint8_t dataLen);
void setConfig(uint8_t *config, uint8_t debounce, uint8_t sensitivity);
void configureI2C();
void testCapacitanceI2C();
void getStatusI2C();

#endif