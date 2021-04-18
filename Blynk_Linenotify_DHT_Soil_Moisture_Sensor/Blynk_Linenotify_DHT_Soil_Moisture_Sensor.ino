#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
void Line_Notify(String message) ;
#include <WiFiClientSecureAxTLS.h>
char auth[] = "******token blynk*******";
char ssid[] = "*******user wifi****";
char pass[] = "********password wifi **********";

// Line config
#define LINE_TOKEN "****** line token *********"

#define DHTTYPE DHT22 
#define DHTPIN D2
DHT dht(DHTPIN, DHTTYPE);

String message = "พบความชื้นในข้าว"; // ArduinoIDE เวอร์ชั่นใหม่ ๆ ใส่ภาษาไทยลงไปได้เลย

WidgetLCD lcd(V2);
BlynkTimer timer;

int ledPin = D0;
int ledPin3 = D1;
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
#define ledGreen D3

void setup() {
 
 // Debug console
  Serial.begin(9600);

  Blynk.begin(auth,ssid,pass);
  //Blynk.begin(auth, ssid, pass, "prakitblog.com", 8181);

  dht.begin();
  pinMode(ledGreen,OUTPUT);

  // Setup a function to be called every second
  timer.setInterval(2000L, soil);
  timer.setInterval(1000L, sendSensor);
  
}

void loop() {
  Blynk.run();
  timer.run();
}

void soil()
{
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "soil moisture is");

  val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog ขา5 ที่ต่อกับ Soil Moisture Sensor Module v1
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  if (val > 500) { 
    //แห้ง
    lcd.print(7, 1, val);
    //Blynk.notify("แห้ง");
  }
  else {
    //ชื้น
    lcd.print(7, 1, val);
    Blynk.notify("พบความชื้นรอบๆบริเวณ");
    Line_Notify(message);
  }
  delay(1000);
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
    
  delay(1000);

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

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }else{
    Serial.println("DHT22 Connected.!!");
    Serial.print("Humidity : ");
    Serial.println(h);
    Serial.print("Temperature : ");
    Serial.println(t);
    delay(1000);
    }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  if(t>30){
  digitalWrite(ledGreen,HIGH);
  //delay(1000);
  }
  else{
  digitalWrite(ledGreen,LOW);
  //delay(1000);
  }
}
