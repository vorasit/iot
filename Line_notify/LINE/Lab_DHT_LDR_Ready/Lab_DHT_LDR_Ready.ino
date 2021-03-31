void Line_Notify(String message) ;
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h> // กรณีขึ้น Error ให้เอาบรรทัดนี้ออก

// Config connect WiFi
#define WIFI_SSID "B4-12_2G"
#define WIFI_PASSWORD "12345678"

// Line config
#define LINE_TOKEN "V6PFkpBbS79vYBnLWjQUuEYAKrhWFOslbSEMtqukTfr"


#define DHTTYPE DHT22 
#define DHTPIN D4
DHT dht(DHTPIN, DHTTYPE);

int led1 = D0; 
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาA0
int val = 0;

String message = "ตรวจพบแสงและเริ่มร้อนแล้ว"; // ArduinoIDE เวอร์ชั่นใหม่ ๆ ใส่ภาษาไทยลงไปได้เลย

void setup() {

  Serial.begin(9600);

  
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  dht.begin();
}

void loop() {
  val = analogRead(analogPin);
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  float t = dht.readTemperature();
  Serial.println(t);
  if ( t > 25.00 && val >400) {
    Line_Notify(message);
 
    Serial.println("ตรวจพบแสงสว่าง");
    Serial.println("TOO HOT CHILLY HOT");
    
    // Serial.println();
  }else {
    //Serial.print("temperature : "); 
    //Serial.print(t);
    Serial.println("Chill"); 
    Serial.println("แสงปกติ");
  }
  delay(5000);
}

void Line_Notify(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
