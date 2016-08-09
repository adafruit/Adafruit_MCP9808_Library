/**************************************************************************/
/*!
  This is a demo for the Adafruit MCP9808 breakout
  ----> http://www.adafruit.com/products/1782
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
*/
/**************************************************************************/

#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup()
{
  Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  Serial.println("MCP9808 demo");

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F

  if (!tempsensor.begin(0x18)) //this is the default i2c addres
  {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1); //don't do anything more.
  }
}

void loop()
{
  Serial.println("Wake up MCP9808.... "); // wake up MSP9808 - power consumption ~200 mikro Ampere
  tempsensor.shutdown_wake(0); // Don't remove this line! required before reading temp, it starts temperature sampling

  tempsensor.setResolution(0); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

  // Read and print out the temperature, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());
  float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();
  Serial.print("Temp: ");
  Serial.print(c, 4); Serial.print("*C and ");
  Serial.print(f, 4); Serial.println("*F.");

  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  Serial.println("");

  delay(2000);
}
