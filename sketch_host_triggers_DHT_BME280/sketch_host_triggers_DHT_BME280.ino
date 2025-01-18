/*
Project:   DHT11, DHT22 und BME280 Temperatur/Luftfeuchte (DHT) und zusätzlich Druck/Höhe (BME280)
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 06.01.2025
Version:   V4.0; Host (PC/LT)triggert Ausgabe mehrerer Sensoren (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_BME280_Library V2.2.4
 - Adafruit_BusIO V1.16.3
 - Adafruit Unified Sensor Library V1.1.15
 - Adafruit_DHT sensor library V1.4.6
 - Backup Arduino Library DHT kxn V3.4.4
 - Makerlabvn_I2C_Motor_Driver V1.0.5 (derzeit nicht in Gebrauch)


Wirering for the BMP280/DHT11/DHT22Sensors:
BMP280	UNO  DHT11 PIN    UNO     DHT22   PIN  UNO
VIN     5.0V VCC   mid    5.0V    VCC    left  5.0V    
GND     GND  GND   right  GND     GND    right GND   
SCL     A5   OUT   left   D7      OUT    mid   D8    
SDA     A4
*/
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define DHTPIN1 7 // Pin für den ersten DHT-Sensor
#define DHTPIN2 8 // Pin für den zweiten DHT-Sensor

Adafruit_BME280 bme; // I2C

const int raum_id1 = 111; // Wert für raum_id des ersten Sensors (111 für DHT11)
const int raum_id2 = 222; // Wert für raum_id des "zweiten" Sensors (222 für DHT22)
const int raum_id3 = 280; // Wert für raum_id des "dritten" Sensors (280 für BME280)

DHT_Unified dht1(DHTPIN1, DHT11);
DHT_Unified dht2(DHTPIN2, DHT22);

void setup() {
  Serial.begin(115200);
  while (!Serial);    // Zeit für den seriellen Monitor
  Serial.println(F("BME280 und DHT Sensoren Test"));

  if (!bme.begin(0x76)) {
    Serial.println("Fehler: kein gültiger BME280 Sensor. Verkabelung prüfen.");
  } else {
    Serial.println("BME280 Sensor erfolgreich initialisiert.");
  }

  dht1.begin();
  dht2.begin();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "GET_DHT11") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent);
      dht1.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Raum: ");
        Serial.print(raum_id1);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Raum: ");
        Serial.print(raum_id1);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
    } else if (command == "GET_DHT22") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des zweiten Sensors messen
      dht2.temperature().getEvent(&tempEvent);
      dht2.humidity().getEvent(&humEvent);

      if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
        // Wenn beide Werte gültig sind, ausgeben
        Serial.print("Raum: ");
        Serial.print(raum_id2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.print("Raum: ");
        Serial.print(raum_id2);
        Serial.println("; Fehler beim Lesen der Sensorwerte!");
      }
    } else if (command == "GET_BME280") {
      // BME280-Sensor auslesen
      Get_BME280();
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
        Serial.print("Raum: ");
        Serial.print(raum_id1);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent1.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent1.relative_humidity);
      } else {
        // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
        Serial.println("Fehler beim Lesen der Sensorwerte!");
      }

      if (!isnan(tempEvent2.temperature) && !isnan(humEvent2.relative_humidity)) {
        Serial.print("Raum: ");
        Serial.print(raum_id2);
        Serial.print("; Temperatur: ");
        Serial.print(tempEvent2.temperature);
        Serial.print("; Feuchte: ");
        Serial.println(humEvent2.relative_humidity);
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
    Serial.print("Raum: ");
    Serial.print(raum_id3);
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