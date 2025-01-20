/****************************************************************
*
* PCF or PCA8574A I²C-Bus-Expander als Ausgabeport
*
****************************************************************/

#include <Wire.h>

#define I2C_ADDR    0x7E >> 1 // I2C-Addresse


void setup() {
  Serial.begin(9600); 
  
  while (!Serial);
  
  Wire.begin(I2C_ADDR); 
  
  Serial.println("Testprogramm PCA8574A");
  Serial.println("I2C LED-BAR");
  Serial.println("---------------------");
  Serial.println();
}


void loop() {
  Serial.println("Pins HIGH schalten");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  Wire.write(0xFF);              // Alle Pins HIGH
  Wire.endTransmission(true);
  
  delay(3000);

  Serial.println("Pins 1, 3, 5, 7 HIGH schalten");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  Wire.write(0x55);              // jedes 2.Pin HIGH
  Wire.endTransmission(true);
  
  delay(3000);

  Serial.println("Pins 2, 4, 6, 8 HIGH schalten");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  Wire.write(0xAA);              // jedes 2.Pin HIGH
  Wire.endTransmission(true);
  
  delay(3000);

  Serial.println("Pins LOW schalten");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  Wire.write(0x00);              // Alle Pins LOW
  Wire.endTransmission(true);
  Serial.println();
    
  delay(3000);
}
