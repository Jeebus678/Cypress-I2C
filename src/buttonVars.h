#ifndef _BUTTONVARS_H_
#define _BUTTONVARS_H_


/* 

        BUTTON SETUP
        ___ POWER ___ 
       |      4      |
  33  MODE           + 8
       |             |
  64  VENT           - 2
       |_____________|

*/

#define REGMAP_ORIGIN			0x00
#define SENSOR_PATTERN			0x00
#define FSS_EN					0x02
#define TOGGLE_EN				0x04
#define LED_ON_EN				0x06
#define SENSITIVITY0			0x08
#define SENSITIVITY1			0x09
#define SENSITIVITY2			0x0A
#define SENSITIVITY3			0x0B
#define BASE_THRESHOLD0			0x0C
#define BASE_THRESHOLD1			0x0D
#define FINGER_THRESHOLD2		0x0E
#define FINGER_THRESHOLD3		0x0F
#define FINGER_THRESHOLD4		0x10
#define FINGER_THRESHOLD5		0x11
#define FINGER_THRESHOLD6		0x12
#define FINGER_THRESHOLD7		0x13
#define FINGER_THRESHOLD8		0x14
#define FINGER_THRESHOLD9		0x15
#define FINGER_THRESHOLD10		0x16
#define FINGER_THRESHOLD11		0x17
#define FINGER_THRESHOLD12		0x18
#define FINGER_THRESHOLD13		0x19
#define FINGER_THRESHOLD14		0x1A
#define FINGER_THRESHOLD15		0x1B
#define SENSOR_DEBOUNCE			0x1C
#define BUTTON_HYS				0x1D
#define BUTTON_BUT				0x1E
#define BUTTON_LBR				0x1F
#define BUTTON_NNT				0x20
#define BUTTON_NT				0x21
#define PROX_EN					0x26
#define PROX_CFG				0x27
#define PROX_CFG2				0x28
#define PROX_TOUCH_TH0			0x2A
#define PROX_TOUCH_TH1			0x2C
#define PROX_HYS				0x30
#define PROX_BUT				0x31
#define PROX_LBR				0x32
#define PROX_NNT				0x33
#define PROX_NT					0x34
#define PROX_POSITIVE_TH0		0x35
#define PROX_POSITIVE_TH1		0x36
#define PROX_NEGATIVE_TH0		0x39
#define PROX_NEGATIVE_TH1		0x3A
#define LED_ON_TIME				0x3D
#define BUZZER_CFG				0x3E
#define BUZZER_ON_TIME			0x3F
#define GPO_CFG					0x40
#define PWM_DUTYCYCLE_CFG0		0x41
#define PWM_DUTYCYCLE_CFG1		0x42
#define PWM_DUTYCYCLE_CFG2		0x43
#define PWM_DUTYCYCLE_CFG3		0x44
#define PWM_DUTYCYCLE_CFG4		0x45
#define PWM_DUTYCYCLE_CFG5		0x46
#define PWM_DUTYCYCLE_CFG6		0x47
#define PWM_DUTYCYCLE_CFG7		0x48
#define SPO_CFG					0x4C
#define DEVICE_CFG0				0x4D
#define DEVICE_CFG1				0x4E
#define DEVICE_CFG2				0x4F
#define I2C_ADDR				0x51
#define REFRESH_CTRL			0x52
#define STATE_TIMEOUT			0x55
#define SLIDER_CFG				0x5D
#define SLIDER1_CFG				0x61
#define SLIDER1_RESOLUTION		0x62
#define SLIDER1_THRESHOLD		0x63
#define SLIDER2_CFG				0x67
#define SLIDER2_RESOLUTION		0x68
#define SLIDER2_THRESHOLD		0x69
#define SLIDER_DEBOUNCE			0x6F
#define SLIDER_BUT				0x70
#define SLIDER_LBR				0x71
#define SLIDER_NNT				0x72
#define SLIDER_NT				0x73
#define CONFIG_CRC				0x7E
#define GPO_OUTPUT_STATE		0x80
#define SENSOR_ID				0x82

/* CTRL_CMD op codes for the command to execute:           
0: There is no command currently executing. The device writes this value at startup and upon
completion of any command.
2: The device calculates a CRC checksum over the configuration data in this register map and
compares the result with the content of CONFIG_CRC. If the two values match, the device saves
the configuration and the CRC checksum to nonvolatile memory.
3: The device calculates a CRC checksum over the configuration data in this register map and
places the result in the CALC_CRC register. Note that this command is only for test and debug,
and its use is not recommended for production configurations. Use EzClick or Host APIs to calculate CRC for production configurations. Refer CY8CMBR3xxx Design Guide for more details.
7: The device discontinues scanning and enters the low power mode. The device will exit this
mode upon an I2C address match event.
8: The device sets the contents of LATCHED_BUTTON_STAT and LATCHED_PROX_STAT to
0 and sets the contents of LIFTOFF_SLIDER1_POSITION and LIFTOFF_SLIDER2_POSITION
to 0xFF.
9: The device resets the Advanced Low Pass filter for proximity sensor PS0
10: The device resets the Advanced Low Pass filter for proximity sensor PS1
255: The device resets itself */
#define CTRL_CMD				0x86

#define CTRL_CMD_ERROR			0x89
#define DEVICE_ID				0x90
#define CALC_CRC_RESULT			0x94
#define BUTTON_STATUS			0xAA
#define PROX_STAT               0xAE
#define DEBUG_SENSOR_ID			0xDC
#define DEBUG_CP				0xDD
#define DEBUG_RAW_COUNT0		0xE2

/* Command Codes */
#define CMD_NULL				0x00
#define SAVE_CHECK_CRC          0x02
#define CALC_CRC                0x03
#define LOAD_FACTORY            0x04
#define LOAD_PRIMARY            0x05
#define LOAD_SECONDARY          0x06
#define SLEEP                   0x07
#define CLEAR_LATCHED_STATUS    0x08
#define CMD_RESET_PROX0_FILTER	0x09
#define CMD_RESET_PROX1_FILTER	0x0A
#define ENTER_CONFIG_MODE       0x0B
#define EXIT_CONTROL_RUN        0xFE
#define SW_RESET                0xFF

/* Total number of configuration registers */
#define TOTAL_CONFIG_REG_COUNT	0x80

/* Length of Register Map */
#define REG_MAP_LEN	256

/* Slave Address (Default) */
#define SLAVE_ADDR	0x37 

#define NO_OF_KIT_BUTTONS	6

#define BUZ_ON_1_33kHz          0x85
#define BUZ_OFF_1_33kHz         0x05
#define SPO_ENABLE              0x03
#define SPO_DISABLE             0x00

#define COMMAND_SUCCESS			0
#define COMMAND_WRITE_FAIL		253
#define COMMAND_CRC_FAIL		254
#define COMMAND_INVALID			255
#define DEVICE_ID_3108			2563
#define DEVICE_ID_3116			2565

#endif