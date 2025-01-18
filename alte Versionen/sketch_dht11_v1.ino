/*
Project:  DHT11 Temperatur und Luftfeuchte Sensor
Author:   Klaus-D. Dittmer mit Hilfe von Co-Pilot
Date:     Created 20.12.2024
Version:  V1.0
IDE:      Arduino IDE 2.3.4 via Windows
 
Required Board (Tools -> Board -> Boards Manager...)
 - Board: Arduino Mega 2560

Required libraries (sketch -> include library -> manage libraries)
 - Adafruit_DHT sensor library V1.4.6
 - Adafruit Unified Sensor Library V1.1.14

Wirering for the DHT11 Sensor:
DHT11       UNO 
VCC         5.0V      
GND         GND 
OUT         D7
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 7        //Pin an dem das OUT Signal angeschlossen ist.
float temp = 0.00;
float hum = 0.00;
DHT_Unified dht(DHTPIN, DHT11);  

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  sensors_event_t event;
  dht.humidity().getEvent(&event);    //Luftfeuchte messen und ausgeben
  hum = event.relative_humidity;
  dht.temperature().getEvent(&event); //Temperatur messen und ausgeben
  temp = event.temperature;
  Serial.print("Temperatur: ");
  Serial.print(temp);
  Serial.print("; Feuchte: ");
  Serial.println(hum);
  delay(15000);    // zwei Sekunden warten, DHT11 Sensor Antwortzeit ist 1 Sek.
}
