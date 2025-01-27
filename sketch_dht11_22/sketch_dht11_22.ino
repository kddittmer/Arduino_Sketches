/*
Project:  Messung con Temperatur und Feuchte mit zwei Sensoren (DHT11 und DHT22)
Author:   Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:     Created 05.01.2025
Version:  V2.2
IDE:      Arduino IDE 2.3.4 via Windows

Required Host:
 - PC oder Laptop
Required Board (Tools -> Board -> Boards Manager...)
 - Board: Arduino UNO
Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_DHT sensor library V1.4.6
 - Adafruit Unified Sensor Library V1.1.14
 - Adafruit Backup Arduino Library 3.4.4

Wirering for the DHT11/22 Sensors:
DHT11 PIN   UNO     DHT22   PIN   UNO 
VCC   mid   5.0V    VCC     left  5.0V      
GND   right GND     GND     right GND 
OUT   left  D7      OUT     mid   D8

*/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT11PIN 7        // Pin an dem das OUT Signal des DHT11 angeschlossen ist.
#define DHT22PIN 8        // Pin an dem das OUT Signal des DHT22 angeschlossen ist.

const int raum_id1 = 111; // Wert für raum_id (111 für DHT11)
const int raum_id2 = 222; // Wert für raum_id (222 für DHT22)

DHT_Unified dht11(DHT11PIN, DHT11);  
DHT_Unified dht22(DHT22PIN, DHT22);  

void setup() {
  Serial.begin(115200);
  dht11.begin();
  dht22.begin();
}

void loop() {
  sensors_event_t tempEvent11, humEvent11;
  sensors_event_t tempEvent22, humEvent22;

  // Temperatur und Luftfeuchtigkeit für DHT11 messen
  dht11.temperature().getEvent(&tempEvent11);
  dht11.humidity().getEvent(&humEvent11);

  if (!isnan(tempEvent11.temperature) && !isnan(humEvent11.relative_humidity)) {
    // Wenn beide Werte gültig sind, ausgeben
    Serial.print("Raum: ");
    Serial.print(raum_id1);
    Serial.print("; Temperatur: ");
    Serial.print(tempEvent11.temperature);
    Serial.print("; Feuchte: ");
    Serial.println(humEvent11.relative_humidity);
  } else {
    // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
    Serial.print("Raum: ");
    Serial.print(raum_id1);
    Serial.println("; Fehler beim Lesen der Sensorwerte!");
  }
  // Verzögerung vor der Abfrage des DHT22 Sensors
  delay(60000); // XX Millisekunden warten

  // Temperatur und Luftfeuchtigkeit für DHT22 messen
  dht22.temperature().getEvent(&tempEvent22);
  dht22.humidity().getEvent(&humEvent22);

  if (!isnan(tempEvent22.temperature) && !isnan(humEvent22.relative_humidity)) {
    // Wenn beide Werte gültig sind, ausgeben
    Serial.print("Raum: ");
    Serial.print(raum_id2);
    Serial.print("; Temperatur: ");
    Serial.print(tempEvent22.temperature);
    Serial.print("; Feuchte: ");
    Serial.println(humEvent22.relative_humidity);
  } else {
    // Wenn einer der Werte ungültig ist, Fehlermeldung ausgeben
    Serial.print("Raum: ");
    Serial.print(raum_id2);
    Serial.println("; Fehler beim Lesen der Sensorwerte!");
  }

  delay(5000);    // XX Millisekunden warten, DHT11 Sensor Antwortzeit ist 1 Sek.
}
