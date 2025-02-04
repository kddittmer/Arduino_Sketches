/*
Project:   DHT11, DHT22 und BME280 Temperatur/Luftfeuchte (DHT) und zusätzlich Druck/Höhe (BME280)
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 06.01.2025
Version:   V4.0; Host (PC/LT/Raspi) triggert Ausgabe des BME280 (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT/Raspi
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_BME280_Library V2.2.4
 - Adafruit_BusIO V1.16.3
 - Adafruit Unified Sensor Library V1.1.15
 - Makerlabvn_I2C_Motor_Driver V1.0.5 (derzeit nicht in Gebrauch)


Wirering for the BMP280/DHT11/DHT22Sensors:
BMP280	UNO  BAR  PIN  UNO
VIN     5.0V VDD    1  5.0V
GND     GND  INT    2  pullup 4k7
SCL     SCL  SCL    3  SCL
SDA     SDA  SDA    4  SDA
             GND    5  GND
*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define LEDBLUE  2 // Pin für blaue LED (Luftdruck)
#define LEDGREEN 3 // Pin für gruene LED (Feuchte)
#define LEDRED   4 // Pin für rote LED (Temperatur)

Adafruit_BME280 bme; // I2C

const int id_sensor6 = 6; // // Sensor_ID für Sensor BME280 (Raum_ID 280, Name: Aussen)

void setup() {
  Serial.begin(115200);
  while (!Serial);    // Zeit für den seriellen Monitor
  Serial.println(F("Info: BME280 und DHT Sensoren Test auf UNO2-Board"));

  if (!bme.begin(0x76)) {
    Serial.println("Info: kein gültiger BME280 Sensor auf UNO2-Board");
  } else {
    Serial.println("Info: BME280 Sensor auf UNO2-Board erfolgreich initialisiert.");
  }

}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "GET_ID6") {
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
    Serial.print(id_sensor6);
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