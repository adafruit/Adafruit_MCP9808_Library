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

#include "Adafruit_MCP9808.h"

/*!
 *    @brief  Instantiates a new MCP9808 class
 */
Adafruit_MCP9808::Adafruit_MCP9808() {}

/*!
 *    @brief  Setups the HW
 *    @param  *theWire
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin(TwoWire *theWire) {
  return begin(MCP9808_I2CADDR_DEFAULT, theWire);
}

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @return True if initialization was successful, otherwise false.
 */

bool Adafruit_MCP9808::begin(uint8_t addr) { return begin(addr, &Wire); }

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @param  *theWire
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin(uint8_t addr, TwoWire *theWire) {
  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(addr, theWire);

  return init();
}

/*!
 *    @brief  Setups the HW with default address
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin() { return begin(MCP9808_I2CADDR_DEFAULT, &Wire); }

/*!
 *    @brief  init function
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::init() {
  if (!i2c_dev->begin()) {
    return false;
  }

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
  float temp = NAN;
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  if (t != 0xFFFF) {
    temp = t & 0x0FFF;
    temp /= 16.0;
    if (t & 0x1000)
      temp -= 256;
  }

  return temp;
}

/*!
 *   @brief  Reads the 16-bit temperature register and returns the Fahrenheit
 *           temperature as a float.
 *   @return Temperature in Fahrenheit.
 */
float Adafruit_MCP9808::readTempF() {
  float temp = NAN;
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  if (t != 0xFFFF) {
    temp = t & 0x0FFF;
    temp /= 16.0;
    if (t & 0x1000)
      temp -= 256;

    temp = temp * 9.0 / 5.0 + 32;
  }

  return temp;
}

/*!
 *   @brief  Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
 *   @param  sw true = shutdown / false = wakeup
 */
void Adafruit_MCP9808::shutdown_wake(boolean sw) {
  uint16_t conf_shutdown;
  uint16_t conf_register = read16(MCP9808_REG_CONFIG);
  if (sw == true) {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
  if (sw == false) {
    conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
}

/*!
 *   @brief  Shutdown MCP9808
 */
void Adafruit_MCP9808::shutdown() { shutdown_wake(true); }

/*!
 *   @brief  Wake up MCP9808
 */
void Adafruit_MCP9808::wake() {
  shutdown_wake(false);
  delay(260);
}

/*!
 *   @brief  Get Resolution Value
 *   @return Resolution value
 */
uint8_t Adafruit_MCP9808::getResolution() {
  return read8(MCP9808_REG_RESOLUTION);
}

/*!
 *   @brief  Set Resolution Value
 *   @param  value
 */
void Adafruit_MCP9808::setResolution(uint8_t value) {
  write8(MCP9808_REG_RESOLUTION, value & 0x03);
}

/*!
 *    @brief  Low level 16 bit write procedures
 *    @param  reg
 *    @param  value
 */
void Adafruit_MCP9808::write16(uint8_t reg, uint16_t value) {
  Adafruit_BusIO_Register reg16 =
      Adafruit_BusIO_Register(i2c_dev, reg, 2, MSBFIRST);

  reg16.write(value);
}

/*!
 *    @brief  Low level 16 bit read procedure
 *    @param  reg
 *    @return value
 */
uint16_t Adafruit_MCP9808::read16(uint8_t reg) {
  Adafruit_BusIO_Register reg16 =
      Adafruit_BusIO_Register(i2c_dev, reg, 2, MSBFIRST);

  return reg16.read();
}

/*!
 *    @brief  Low level 8 bit write procedure
 *    @param  reg
 *    @param  value
 */
void Adafruit_MCP9808::write8(uint8_t reg, uint8_t value) {
  Adafruit_BusIO_Register reg8 = Adafruit_BusIO_Register(i2c_dev, reg, 1);

  reg8.write(value);
}

/*!
 *    @brief  Low level 8 bit read procedure
 *    @param  reg
 *    @return value
 */
uint8_t Adafruit_MCP9808::read8(uint8_t reg) {
  Adafruit_BusIO_Register reg8 = Adafruit_BusIO_Register(i2c_dev, reg, 1);

  return reg8.read();
}

/**************************************************************************/
/*!
    @brief  Gets the pressure sensor and temperature values as sensor events

    @param  temp Sensor event object that will be populated with temp data
    @returns True
*/
/**************************************************************************/
bool Adafruit_MCP9808::getEvent(sensors_event_t *temp) {
  uint32_t t = millis();

  // use helpers to fill in the events
  memset(temp, 0, sizeof(sensors_event_t));
  temp->version = sizeof(sensors_event_t);
  temp->sensor_id = _sensorID;
  temp->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  temp->timestamp = t;
  temp->temperature = readTempC();
  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the overall sensor_t data including the type, range and
   resulution
    @param  sensor Pointer to Adafruit_Sensor sensor_t object that will be
   filled with sensor type data
*/
/**************************************************************************/
void Adafruit_MCP9808::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "MCP9808", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  sensor->min_delay = 0;
  sensor->max_value = 100.0;
  sensor->min_value = -20.0;
  sensor->resolution = 0.0625;
}
/*******************************************************/
