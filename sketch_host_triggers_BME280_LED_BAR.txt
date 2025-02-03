/*
Project:   BME280 Temperatur/Luftfeuchte/Druck/Höhe und Anzeige
Author:    Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:      Created 02.02.2025
Version:   V5.0; Host (PC/LT/Raspi)triggert Ausgabe mehrerer Sensoren (Duplex Betrieb)
IDE:       Arduino IDE 2.3.4 via Windows

Required Host:
  - PC/LT/Raspi
Required Board (Tools -> Board -> Boards Manager...)
  - Board: Arduino UNO

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_BME280_Library V2.2.4
 - Adafruit_BusIO V1.16.3
 - Adafruit Unified Sensor Library V1.1.15
 - Backup Arduino Library DHT kxn V3.4.4
 - Makerlabvn_I2C_Motor_Driver V1.0.5 (derzeit nicht in Gebrauch)


Wirering for the DHT22/BME280/LED-Bargraph:
DHT22   PIN  UNO  | BME280	UNO  | BAR  PIN  UNO
VCC    left  5.0V | VIN     5.0V | VDD    1  5.0V  
GND    right GND  | GND     GND  | INT    2  pullup 4k7   
OUT    mid   D2   | SCL     SCL  | SCL    3  SCL   
                  | SDA     SDA  | SDA    4  SDA
                                 | GND    5  GND
*/
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define DHTPIN1 2 // Pin für den DHT-Sensor falls vorhanden
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

const int sensor_id5 = 5; // Sensornummer für DHT22 in Raum 22X
const int sensor_id6 = 6; // Sensornummer für BME280 in Raum 280

#define I2C_ADDR_BAR 0x20 // Adresse des LED-Bargraph

DHT_Unified dht1(DHTPIN1, DHT22);

const int LED5rt = 3; // Pin für Temperatur-LED von Sensor 5
const int LED5bl = 4; // Pin für Feuchte-LED von Sensor 5
const int LED6rt = 5; // Pin für Temperatur-LED von Sensor 6
const int LED6bl = 6; // Pin für Feuchte-LED von Sensor 6
const int LED6gn = 7; // Pin für Druck-LED von Sensor 6

void setup() {
  Serial.begin(115200);
  while (!Serial);    // Zeit für den seriellen Monitor
  Serial.print(F("Info; BME280 Test: "));

  if (!bme.begin(0x76)) {
    Serial.println("Info: kein gültiger BME280 Sensor im Board.");
  } else {
    Serial.println("Info: BME280 Sensor erfolgreich initialisiert.");
  }

  pinMode(LED5rt, OUTPUT);
  pinMode(LED5bl, OUTPUT);
  pinMode(LED6rt, OUTPUT);
  pinMode(LED6bl, OUTPUT);
  pinMode(LED6gn, OUTPUT);

  Wire.begin();

  // Initialisiere den Bargraph
  Wire.beginTransmission(I2C_ADDR_BAR);
  Wire.write(0x00); // Register 0x00 auswählen
  Wire.write(0xFF); // Alle Ports als Ausgang konfigurieren
  Wire.endTransmission(true);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "GET_ID5") {
      sensors_event_t tempEvent, humEvent;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent);
      dht1.humidity().getEvent(&humEvent);

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
      // BME280-Sensor auslesen
      Get_BME280();
    } else if (command == "GET_ALL") {
      sensors_event_t tempEvent1, humEvent1;

      // Temperatur und Luftfeuchtigkeit des ersten Sensors messen
      dht1.temperature().getEvent(&tempEvent5);
      dht1.humidity().getEvent(&humEvent5);

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
      // BME280-Sensor auslesen
      Get_BME280();
    } else if (command.startsWith("SET5_")) {
      // Hier könnte zukünftiger Code für Sensor 5 stehen
    } else if (command.startsWith("SET6_")) {
      processSetCommand(command.substring(5), 6);
    }
  }
}

void Get_BME280() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA); // Anpassung der Höhe

  if (!isnan(temperature) && !isnan(pressure) && !isnan(altitude) && !isnan(humidity)) {
    // Wenn alle Werte gültig sind, ausgeben
    Serial.print("Sensor: ");
    Serial.print(sensor_id6);
    Serial.print("; Temperatur: ");
    Serial.print(temperature);
    Serial.print("; Feuchte: ");
    Serial.print(humidity);
    Serial.print("; Druck: ");
    Serial.println(pressure);
  } else {
    // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
    Serial.println("Fehler beim Lesen der Sensorwerte vom BME280!");
  }
}

void setLEDBar(int barIndex, int value) {
  Wire.beginTransmission(I2C_ADDR_BAR);
  Wire.write(I2C_ADDR_BAR & 0xFE);   // Alle Ports als Ausgang
  Wire.write(~value); // Invertiere den Wert für negative Logik
  Wire.endTransmission();
}

void processSetCommand(String command, int sensor_id) {
  if (sensor_id == 6) {
    String values[3];
    int index = 0;
    int lastPos = 0;
    for (int i = 0; i < command.length(); i++) {
      if (command.charAt(i) == '_') {
        values[index++] = command.substring(lastPos, i);
        lastPos = i + 1;
      }
    }
    values[index] = command.substring(lastPos);

    for (int i = 0; i <= index; i++) {
      String valueStr = values[i];
      char metric = valueStr.charAt(0);
      String trend = valueStr.substring(2);

      int ledValue = 0;
      if (trend == "DN") {
        ledValue = 0x07;
      } else if (trend == "UP") {
        ledValue = 0xE0;
      } else if (trend == "EQ") {
        ledValue = 0x18;
      }

      if (metric == 'rt') {
        digitalWrite(LED6rt, HIGH);
        setLEDBar(I2C_ADDR_BAR, ledValue);
        delay(5000);
        digitalWrite(LED6rt, LOW);
      } else if (metric == 'bl') {
        digitalWrite(LED6bl, HIGH);
        setLEDBar(I2C_ADDR_BAR, ledValue);
        delay(5000);
        digitalWrite(LED6bl, LOW);
      } else if (metric == 'gn') {
        digitalWrite(LED6gn, HIGH);
        setLEDBar(I2C_ADDR_BAR, ledValue);
        delay(5000);
        digitalWrite(LED6gn, LOW);
      }
    }
  }
}

