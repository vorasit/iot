void Line_Notify(String message) ;

#include <ESP8266WiFi.h>
#include <WiFiClientSecureAxTLS.h> // กรณีขึ้น Error ให้เอาบรรทัดนี้ออก

// Config connect WiFi
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// Line config
#define LINE_TOKEN "cCwCg5SNIIQXsO7bs36oOeSe64dQmC98CPM"

int led1 = D0; 
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาA0
int val = 0;

String message = "ตรวจพบแสงสว่าง"; // ข้อความที่ส่งไปแจ้งเตือนที่ LINE

void setup() {
  
  pinMode(led1, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา 2 เป็น OUTPUT 

digitalWrite(led1,LOW);

  Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {

   val = analogRead(analogPin);
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  if (val > 400) {
    Line_Notify(message);
    digitalWrite(led1,HIGH);
    delay(3000);
    digitalWrite(led1,LOW);
   Serial.println("ตรวจพบแสงสว่าง");
    

    delay(3000);
  }
  else {
    
    // digitalWrite(led1,LOW);
     Serial.println("ปกติ");
      
    }
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
