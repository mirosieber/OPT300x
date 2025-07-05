/*

This is example for OPT300x Digital Ambient Light Sensor Family
Tested on OPT 3001 and 3004

Initial Date: 02-Dec-2015
Last Eddit: 05.07.2025 bs MS 	added ESP32 suport generalised frome OPT3001 to
OPT300x Family

Hardware connections for Arduino Uno:
VDD to 3.3V DC
SDA to A4
SCL to A5
GND to common ground

Written by AA for ClosedCube

MIT License

*/

#include "OPT300x.h"
#include <Wire.h>

OPT300x opt300x;

#define OPT300x_ADDRESS 0x44

// to suport ESP32 pin assignment, I2C pins need to be defined
#ifdef ESP32
#define I2C_SDA 6
#define I2C_SCL 7
#endif

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("OPT300x Test");
  delay(2000);
#ifdef ESP32
  Wire.begin(I2C_SDA, I2C_SCL);
#else
  Wire.begin();
#endif
  opt300x.begin(OPT300x_ADDRESS);
  Serial.print("OPT300x Manufacturer ID");
  Serial.println(opt300x.readManufacturerID());
  Serial.print("OPT300x Device ID");
  Serial.println(opt300x.readDeviceID());

  configureSensor();
  printResult("High-Limit", opt300x.readHighLimit());
  printResult("Low-Limit", opt300x.readLowLimit());
  Serial.println("----");
}

void loop() {
  OPT300x_S result = opt300x.readResult();
  printResult("OPT300x", result);
  delay(500);
}

void configureSensor() {
  OPT300x_Config newConfig;

  newConfig.RangeNumber = B1100;
  newConfig.ConvertionTime = B0;
  newConfig.Latch = B1;
  newConfig.ModeOfConversionOperation = B11;

  OPT300x_ErrorCode errorConfig = opt300x.writeConfig(newConfig);
  if (errorConfig != NO_ERROR)
    printError("OPT300x configuration", errorConfig);
  else {
    OPT300x_Config sensorConfig = opt300x.readConfig();
    Serial.println("OPT300x Current Config:");
    Serial.println("------------------------------");

    Serial.print("Conversion ready (R):");
    Serial.println(sensorConfig.ConversionReady, HEX);

    Serial.print("Conversion time (R/W):");
    Serial.println(sensorConfig.ConvertionTime, HEX);

    Serial.print("Fault count field (R/W):");
    Serial.println(sensorConfig.FaultCount, HEX);

    Serial.print("Flag high field (R-only):");
    Serial.println(sensorConfig.FlagHigh, HEX);

    Serial.print("Flag low field (R-only):");
    Serial.println(sensorConfig.FlagLow, HEX);

    Serial.print("Latch field (R/W):");
    Serial.println(sensorConfig.Latch, HEX);

    Serial.print("Mask exponent field (R/W):");
    Serial.println(sensorConfig.MaskExponent, HEX);

    Serial.print("Mode of conversion operation (R/W):");
    Serial.println(sensorConfig.ModeOfConversionOperation, HEX);

    Serial.print("Polarity field (R/W):");
    Serial.println(sensorConfig.Polarity, HEX);

    Serial.print("Overflow flag (R-only):");
    Serial.println(sensorConfig.OverflowFlag, HEX);

    Serial.print("Range number (R/W):");
    Serial.println(sensorConfig.RangeNumber, HEX);

    Serial.println("------------------------------");
  }
}

void printResult(String text, OPT300x_S result) {
  if (result.error == NO_ERROR) {
    Serial.print(text);
    Serial.print(": ");
    Serial.print(result.lux);
    Serial.println(" lux");
  } else {
    printError(text, result.error);
  }
}

void printError(String text, OPT300x_ErrorCode error) {
  Serial.print(text);
  Serial.print(": [ERROR] Code #");
  Serial.println(error);
}
