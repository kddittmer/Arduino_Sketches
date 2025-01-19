/*
Project:   DHT11 und DHT22 Temperatur und Luftfeuchte Sensor
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 12.01.2025
Version:   V3.5; Host (PC/LT)triggert Ausgabe mehrerer Sensoren (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_DHT sensor library V1.4.6
 - Adafruit Unified Sensor Library V1.1.15
 - DHT sensor library
 - Backup Arduino Library DHT kxn (ggf. nicht erforderlich)


Wirering for the DHT11/DHT22Sensors:
DHT11 PIN    UNO     DHT22   PIN  UNO
VCC   mid    5.0V    VCC    left  5.0V
GND   right  GND     GND    right GND
OUT   left   D7      OUT    mid   D8

*/
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define DHTPIN5 7            // Pin für den Sensor DHT11
#define DHTPIN6 8            // Pin für den Sensor DHT22

const int sensor_id5 = 5; // Sensornummer für DHT22 in Raum 322
const int sensor_id6 = 6; // Sensornummer für DHTXX in Raum 4XX

DHT_Unified dht5(DHTPIN5, DHT22);
DHT_Unified dht6(DHTPIN6, DHT22);

void setup() {
  Serial.begin(115200);
  dht5.begin();
  dht6.begin();
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
        Serial.println("Fehler beim Lesen der Sensorwerte!");
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
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }
    }
  }
}
