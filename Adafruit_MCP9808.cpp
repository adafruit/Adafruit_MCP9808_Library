/*!
 *  @file Adafruit_MCP9808.cpp
 *
 *  @mainpage Adafruit MCP9808 I2C Temp Sensor
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for Microchip's MCP9808 I2C Temp sensor
 *
 * 	This is a library for the Adafruit MCP9808 breakout:
 * 	http://www.adafruit.com/products/1782
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section author Author
 *
 *  K.Townsend (Adafruit Industries)
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "Adafruit_MCP9808.h"

/*!
 *    @brief  Instantiates a new MCP9808 class
 */
Adafruit_MCP9808::Adafruit_MCP9808() {}

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_MCP9808::begin(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();

  if (read16(MCP9808_REG_MANUF_ID) != 0x0054)
    return false;
  if (read16(MCP9808_REG_DEVICE_ID) != 0x0400)
    return false;

  write16(MCP9808_REG_CONFIG, 0x0);
  return true;
}

/*!
 *   @brief  Reads the 16-bit temperature register and returns the Centigrade
 *           temperature as a float.
 *   @return Temperature in Centigrade.
 */
float Adafruit_MCP9808::readTempC() {
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF;
  temp /= 16.0;
  if (t & 0x1000)
    temp -= 256;

  return temp;
}

/*!
 *   @brief  Reads the 16-bit temperature register and returns the Fahrenheit
 *           temperature as a float.
 *   @return Temperature in Fahrenheit.
 */
float Adafruit_MCP9808::readTempF() {
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF;
  temp /= 16.0;
  if (t & 0x1000)
    temp -= 256;

  temp = temp * 9.0 / 5.0 + 32;

  return temp;
}

/*!
 *   @brief  Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
 *   @param  sw_ID
 *           1 = shutdown / 0 = wakeup
 */
void Adafruit_MCP9808::shutdown_wake(uint8_t sw_ID) {
  uint16_t conf_shutdown;
  uint16_t conf_register = read16(MCP9808_REG_CONFIG);
  if (sw_ID == 1) {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
  if (sw_ID == 0) {
    conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
}

/*!
 *   @brief  Shutdown MCP9808
 */
void Adafruit_MCP9808::shutdown() { shutdown_wake(1); }

/*!
 *   @brief  Wake up MCP9808
 */
void Adafruit_MCP9808::wake() {
  shutdown_wake(0);
  delay(250);
}

/*!
 *    @brief  Low level 16 bit write procedures
 *    @param  reg
 *    @param  value
 */
void Adafruit_MCP9808::write16(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.write(value >> 8);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

/*!
 *    @brief  Low level 16 bit read procedure
 *    @param  reg
 *    @return value
 */
uint16_t Adafruit_MCP9808::read16(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
  val = Wire.read();
  val <<= 8;
  val |= Wire.read();
  return val;
}
