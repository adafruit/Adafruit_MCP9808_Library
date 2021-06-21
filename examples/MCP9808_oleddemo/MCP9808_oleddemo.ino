#include <Adafruit_SH110X.h>
#include <Adafruit_MCP9808.h>
#include <Fonts/FreeSans9pt7b.h>


Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

Adafruit_MCP9808  sensor;

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  
  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  if (!sensor.begin()) {
    Serial.println("Failed to find Adafruit MCP9808 chip");
    while (1) { delay(10); }
  }
  
  Serial.println("MCP9808 Found!");
  
  display.setRotation(1);
  display.setFont(&FreeSans9pt7b);
  display.setTextColor(SH110X_WHITE);  
}

void loop() {
  sensors_event_t event; 
  sensor.getEvent(&event);
  
  Serial.print("Temperature: ");
  Serial.print(event.temperature);
  Serial.println(" degrees C");
  Serial.println("");

  display.clearDisplay();
  display.setCursor(0, 25);
  display.println("~ MCP9808 ~");
  display.print("Temp: "); 
  display.print(event.temperature); 
  display.println(" C");

  display.display();
  yield();
  delay(100);
}
