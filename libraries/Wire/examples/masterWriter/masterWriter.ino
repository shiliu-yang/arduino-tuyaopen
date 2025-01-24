#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200); // start serial for output
  Wire.begin();         // join i2c bus (address optional for master)
}

void loop()
{
  static uint8_t x = 0;

  Serial.println("Sending: " + String(x));
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(x);             // sends one byte
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}
