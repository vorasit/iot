#include "DHT.h"
 #define DHTPIN D2 
 #define DHTTYPE DHT22 
 DHT dht(DHTPIN, DHTTYPE);
 void setup() {
 Serial.begin(115200);
 Serial.println(F("DHTxx test!"));
 dht.begin();
 }
 void loop() { 
 delay(2000);
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 float f = dht.readTemperature(true); 
 if (isnan(h) || isnan(t) || isnan(f)) {
 Serial.println(F("Failed to read from DHT sensor!"));
 return;
 }
 float hif = dht.computeHeatIndex(f, h);
 float hic = dht.computeHeatIndex(t, h, false);
 Serial.print(F("Humidity: "));
 Serial.print(h);
 Serial.print(F("% Temperature: "));
 Serial.print(t);
 Serial.println(F("°C "));
 }
