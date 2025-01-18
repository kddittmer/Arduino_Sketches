/*
Project:   DHT11, DHT22 und BME280 Temperatur/Luftfeuchte (DHT) und zusätzlich Druck/Höhe (BME280)
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 06.01.2025
Version:   V4.1; Host (PC/LT)triggert Ausgabe mehrerer Sensoren (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_BME280_Library V2.2.4
 - Adafruit_BusIO V1.16.3
 - Adafruit Unified Sensor Library V1.1.15
 - DHT sensor library (Adafruit) V1.4.6
 - DHT_knx Backup Arduino Library V3.4.4
 - Makerlabvn_I2C_Motor_Driver V1.0.5 (derzeit nicht in Gebrauch)


Wirering for the BMP280/DHT11/DHT22Sensors:
BMP280	MEGA DHT11 PIN    MEGA   DHT22_1 PIN   MEGA DHT22_2 PIN   MEGA
VIN     5.0V VCC   mid    5.0V   VCC     left  5.0V VCC     left  5.0V
GND     GND  GND   right  GND    GND     right GND  GND     right GND
SCL     SCL  OUT   left   D2     OUT     mid   D3   OUT     mid   D4
SDA     SDA
*/
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define DHTPIN1 2 // Pin für den ersten DHT-Sensor (DHT11)
#define DHTPIN2 3 // Pin für den zweiten DHT-Sensor (DHT22_1)
#define DHTPIN3 4 // Pin für den dritten DHT-Sensor (DHT22_2)

Adafruit_BME280 bme; // I2C für den vierten Sensor (BME280)

const int id_sensor1 = 1; // Sensor-ID für Raum 111 (DHT11)
const int id_sensor2 = 2; // Sensor-ID für Raum 221 (DHT22)
const int id_sensor3 = 3; // Sensor-ID für Raum 222 (DHT22)
const int id_sensor4 = 4; // Sensor-ID für Raum 280 (BME280)


DHT_Unified dht1(DHTPIN1, DHT11);
DHT_Unified dht2(DHTPIN2, DHT22);
DHT_Unified dht3(DHTPIN3, DHT22);

void setup() {
  Serial.begin(115200);
  while (!Serial);    // Zeit für den seriellen Monitor
  Serial.println(F("Info: BME280 und DHT Sensoren Test"));

  if (!bme.begin(0x76)) {
    Serial.println("Info: kein gültiger BME280 Sensor. Verkabelung prüfen.");
  } else {
    Serial.println("Info: BME280 Sensor erfolgreich initialisiert.");
  }

  dht1.begin();
  dht2.begin();
  dht3.begin();
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
        Serial.print(id_sensor1);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(id_sensor1);
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
        Serial.print(id_sensor2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(id_sensor2);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
     } else if (command == "GET_ID3") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des dritten Sensors messen
      dht3.temperature().getEvent(&tempEvent);
      dht3.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(id_sensor3);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Sensor: ");
        Serial.print(id_sensor3);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
     } else if (command == "GET_ID4") {
      // BME280-Sensor auslesen
      Get_BME280();
     } else if (command == "GET_ALL") {
      sensors_event_t tempEvent1, humEvent1, tempEvent2, humEvent2, tempEvent3, humEvent3;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent1);
      dht1.humidity().getEvent(&humEvent1);

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht2.temperature().getEvent(&tempEvent2);
      dht2.humidity().getEvent(&humEvent2);

      // Temperatur und Luftfeuchtigkeit des dritten Sensors messen
      dht3.temperature().getEvent(&tempEvent3);
      dht3.humidity().getEvent(&humEvent3);

      if (!isnan(tempEvent1.temperature) && !isnan(humEvent1.relative_humidity)) {
        // Wenn alle Werte gültig sind, ausgeben
        Serial.print("Sensor: ");
        Serial.print(id_sensor1);
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
        Serial.print(id_sensor2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent2.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent2.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }

      if (!isnan(tempEvent3.temperature) && !isnan(humEvent3.relative_humidity)) {
        Serial.print("Sensor: ");
        Serial.print(id_sensor3);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent3.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent3.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }

      // BME280-Sensor auslesen
      Get_BME280();
    }
  }
}

void Get_BME280() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA); // Anpassung der Höhe

  //Serial.println("Get_BME280() aufgerufen.");

  if (!isnan(temperature) && !isnan(pressure) && !isnan(altitude) && !isnan(humidity)) {
    // Wenn alle Werte gültig sind, ausgeben
    Serial.print("Sensor: ");
    Serial.print(id_sensor4);
    Serial.print("; Temperatur: ");
    Serial.print(temperature);
    Serial.print("; Feuchte: ");
    Serial.print(humidity);
    Serial.print("; Druck: ");
    Serial.println(pressure);
    //Serial.print("; Höhe: ");
    //Serial.print(altitude);
    //Serial.println(" m");
  } else {
    // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
    Serial.println("Fehler beim Lesen der Sensorwerte vom BME280!");
  }
}