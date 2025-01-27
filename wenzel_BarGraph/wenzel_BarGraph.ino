/****************************************************************
*
* PCF or PCA8574A I²C-Bus-Expander als Ausgabeport
*
****************************************************************/

#include <Wire.h>

#define I2C_ADDR    0x20 // I2C-Addresse
/* Die Adresse kann mit Hilfe der DIP-Switches A0-A2 zwischen 
*  20 und 27 angepasst werden. Der I2C Adress-Scanner der Wire.h
*  Bibliothek stellt neben dieser Adresse auch immer noch 0x7C
*  als I2C Adresse fest. Keine Ahnung, was das bedeutet.
*  Mit dem LCD-Display muss dann ein Konflikt vermieden werden.
*/

void setup() {
  Serial.begin(115200); 
    while (!Serial);
    Wire.begin(I2C_ADDR); 
    Serial.println("Testprogramm PCA8574A");
    Serial.println("I2C LED-BAR");
    Serial.println("=====================");
    Serial.println("Lauflicht");
    Serial.println();
    delay(1000);
   //Serial.println("Pins Low schalten");
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
    Wire.write(0x00);              // Alle LED's EIN
    Wire.endTransmission(true);
    delay (1000);
  //Serial.println("Pins High schalten");
    Wire.endTransmission(true);
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
    Wire.write(0xFF);              // Alle LED AUS
    Wire.endTransmission(true);
    delay(2000);
}

void loop() {
  //delay(200)
  //Serial.print("Pins HIGH schalten: Adresse: ");
  //Serial.println(I2C_ADDR);
  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  //Wire.write(0x00);              // Alle LED EIN
  //Wire.endTransmission(true);
  
  //delay(1000);

  //Serial.println("Pins 1, 3, 5, 7 HIGH schalten");
  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  //Wire.write(0x55);              // jedes 2.Pin HIGH
  //Wire.endTransmission(true);
  
  //delay(200);

  //Serial.println("Pins 2, 4, 6, 8 HIGH schalten");
  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  //Wire.write(0xAA);              // jedes 2.Pin HIGH
  //Wire.endTransmission(true);
  
  //delay(200);

  //Serial.println("Pins LOW schalten");
  //Wire.beginTransmission(I2C_ADDR);
  //Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
  //Wire.write(0xFF);              // Alle LED AUS
  //Wire.endTransmission(true);
  //Serial.println();
  //delay(1000);
  for (int i = 0; i < 8; i++){
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
    Wire.write(~(0x01 << i)); // Setze Schleifenzähler
    Wire.endTransmission(true);
    //Serial.println(~(0x01 << i), BIN); // Ausgabe des Bitmusters zur Überprüfung
    delay(20);
  }
    for (int i = 7; i >=0 ; i--){
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(I2C_ADDR & 0xFE);   // Alle Ports als Ausgang
    Wire.write(~(0x01 << i)); // Setze Schleifenzähler
    Wire.endTransmission(true);
    //Serial.println(~(0x80 >> i), BIN); // Ausgabe des Bitmusters zur Überprüfung
    delay(20);
  }

  delay(10);
}
