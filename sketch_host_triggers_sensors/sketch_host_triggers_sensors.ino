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

#define DHTPIN1 7            // Pin für den Sensor DHT11
#define DHTPIN2 8            // Pin für den Sensor DHT22

const int sensor_id1 = 1; // Sensornummer für DHT11 in Raum 111
const int sensor_id2 = 2; // Sensornummer für DHT22 in Raum 221

DHT_Unified dht1(DHTPIN1, DHT11);
DHT_Unified dht2(DHTPIN2, DHT22);

void setup() {
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "GET_ID1") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent);
      dht1.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id1);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id1);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
    } else if (command == "GET_ID2") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht2.temperature().getEvent(&tempEvent);
      dht2.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id2);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }

    } else if (command == "GET_ALL") {
      sensors_event_t tempEvent1, humEvent1, tempEvent2, humEvent2;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent1);
      dht1.humidity().getEvent(&humEvent1);

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht2.temperature().getEvent(&tempEvent2);
      dht2.humidity().getEvent(&humEvent2);

      if (!isnan(tempEvent1.temperature) && !isnan(humEvent1.relative_humidity)) {
        // Wenn alle Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(sensor_id1);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent1.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent1.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }

      if (!isnan(tempEvent2.temperature) && !isnan(humEvent2.relative_humidity)) {
        Serial.print("Sensor: ");
        Serial.print(sensor_id2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent2.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent2.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }
    }
  }
}