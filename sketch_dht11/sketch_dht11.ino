/*
Project:  DHT11 Temperatur und Luftfeuchte Sensor
Author:   Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:     Created 20.12.2024
Version:  V2.1
IDE:      Arduino IDE 2.3.4 via Windows

Required Host:
 - PC oder Laptop
Required Board (Tools -> Board -> Boards Manager...)
 - Board: Arduino UNO
Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_DHT sensor library V1.4.6
 - Adafruit Unified Sensor Library V1.1.14
 - Adafruit Backup Arduino Library 3.4.4

Wirering for the DHT11 Sensor:
DHT11       UNO 
VCC         5.0V      
GND         GND 
OUT         D7
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHT11PIN 7        	//Pin an dem das OUT Signal des DHT11 angeschlossen ist.
const int raum111_id = 111;	// Wert für raum_id (111 für DHT11; 222 für DHT22; DHT221 kommt vom Mega-Board )
DHT_Unified dht11(DHT11PIN, DHT11);  
//DHT_Unified dht(DHT22PIN, DHT22);  

void setup() {
  Serial.begin(115200);
  dht11.begin();
}

void loop() {
  sensors_event_t tempEvent, humEvent;

  // Temperatur und Luftfeuchtigkeit messen
  dht11.temperature().getEvent(&tempEvent);
  dht11.humidity().getEvent(&humEvent);

  if (!isnan(tempEvent.temperature) && !isnan(humEvent.relative_humidity)) {
    // Wenn beide Werte gueltig sind, ausgeben
    Serial.print("Raum: ");
    Serial.print(raum111_id);
    Serial.print("; Temperatur: ");
    Serial.print(tempEvent.temperature);
    Serial.print("; Feuchte: ");
    Serial.println(humEvent.relative_humidity);
  } else {
    // Wenn einer der Werte ungueltig ist, Fehlermeldung ausgeben
    Serial.print("Raum: ");
    Serial.print(raum111_id);
    Serial.println("; Fehler beim Lesen der Sensorwerte!");
  }

  delay(10000);    // XX Millsekunden warten, DHT11 Sensor Antwortzeit ist 1 Sek.
}