/*
Project:   DHT11 und DHT22 Temperatur und Luftfeuchte Sensor
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 12.01.2025
Version:   V4.0; Host (PC/LT/Raspi) triggert Ausgabe mehrerer Sensoren und Anzeige-LED-Bars (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT/RASPI
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO/Mega

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_DHT sensor library V1.4.6
 - Adafruit Unified Sensor Library V1.1.15
 - DHT sensor library
 - Backup Arduino Library DHT kxn (ggf. nicht erforderlich)


Wirering for the DHT11/DHT22 Sensors and LED-BAR:
DHT11 PIN    UNO     DHT22   PIN  UNO   BAR  PIN  UNO
VCC   mid    5.0V    VCC    left  5.0V  VDD    1  5.0V
GND   right  GND     GND    right GND   INT    2  pullup 4k7
OUT   left   D7      OUT    mid   D8    SCL    3  SCL
                                        SDA    4  SDA
                                        GND    5  GND

*/
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define DHTPIN5 7            // Pin für Sensor DHT22
#define DHTPIN6 8            // Pin für den Sensor DHTXX2

const int sensor_id5 = 5; // Sensornummer für DHT22 in Raum 322
const int sensor_id6 = 6; // Sensornummer für DHTXX in Raum 4XX

#define I2C_ADDR_BAR5 0x20
#define I2C_ADDR_BAR6 0x21

DHT_Unified dht5(DHTPIN5, DHT22);
DHT_Unified dht6(DHTPIN6, DHT22);

void setup() {
  Serial.begin(115200);
  dht5.begin();
  dht6.begin();
  Wire.begin();
  Wire.setClock(400000); // Setze die I2C-Taktfrequenz auf 400 kHz (optional)
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "GET_ID5") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht5.temperature().getEvent(&tempEvent);
      dht5.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id5);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id5);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
    } else if (command == "GET_ID6") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht6.temperature().getEvent(&tempEvent);
      dht6.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id6);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id6);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }

    } else if (command == "GET_ALL") {
      sensors_event_t tempEvent5, humEvent5, tempEvent6, humEvent6;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht5.temperature().getEvent(&tempEvent5);
      dht5.humidity().getEvent(&humEvent5);

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht6.temperature().getEvent(&tempEvent6);
      dht6.humidity().getEvent(&humEvent6);

      if (!isnan(tempEvent5.temperature) && !isnan(humEvent5.relative_humidity)) {
        // Wenn alle Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id5);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent5.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent5.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id5);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");      
      }

      if (!isnan(tempEvent6.temperature) && !isnan(humEvent6.relative_humidity)) {
        Serial.print("Sensor: ");
        Serial.print(sensor_id6);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent6.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent6.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id6);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");      
      }
    } else if (command.startsWith("BAR5_")) {
      String valueStr = command.substring(5);
      int value = (int) strtol(valueStr.c_str(), NULL, 16);
      setLEDBar(I2C_ADDR_BAR5, ~value); // Invertiere den Wert für positive Logik
    } else if (command.startsWith("BAR6_")) {
      String valueStr = command.substring(5);
      int value = (int) strtol(valueStr.c_str(), NULL, 16);
      setLEDBar(I2C_ADDR_BAR6, ~value); // Invertiere den Wert für positive Logik
    }
  }
}

void setLEDBar(uint8_t address, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(address & 0xFE);   // Alle Ports als Ausgang
  Wire.write(value);            // Setze den Wert
  Wire.endTransmission(true);
}
