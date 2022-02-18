#include <Arduino.h>
#include "i2cHandler.h"

#define BUTTON_INTERRUPT 0

void IRAM_ATTR buttonInput()
{
	Serial.println("Button Pressed.");
}

void setup()
{
	Serial.begin(115200);
	while (!Serial){};
	initI2C();
	scanI2C();
	configureI2C();
}

void loop()
{
	delay(3000);
	getStatusI2C(); 
}