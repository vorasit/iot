#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <Servo.h>
#include <TridentTD_LineNotify.h>
#define SSID        "............."      // บรรทัดที่ 11 ให้ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "..............."     // บรรทัดที่ 12 ใส่ รหัส Wifi
#define LINE_TOKEN  "..........token.............."   // บรรทัดที่ 13 ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

#define BLYNK_PRINT Serial
char auth[] = "........blynk............";
SimpleTimer timer;

Servo servo;

int analogPin = A0;
int val = 0;
void setup() {
  servo.attach(5); //D1
  servo.write(0);
  Blynk.begin(auth, SSID, PASSWORD);
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  LINE.notify("กำลังทำงาน");
  timer.setInterval(1000L, getSendData);
}

void loop() {
  delay(2000);
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
  val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog 
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val
  if (val >= 700) {
    String LineText;
    String string1 = "ลืมปิดแก๊ส";
    String string2 = " !!";
    LineText = string1+ string2;
    Serial.print("Line ");
    Serial.println(LineText);
    LINE.notify(LineText);
    servo.write(90);
    delay(2000);
  }
  if(val < 700 && val >= 320){
    String LineText;
    String string1 = "ปิดแก๊สสำเร็จ 🙂";
    LineText = string1;
    Serial.print("Line ");
    Serial.println(LineText);
    LINE.notify(LineText);
    servo.write(0);
    delay(2000);
    }
}
void getSendData()
{
  Blynk.virtualWrite(V2, val);
  if (val > 700 )
  {
    Blynk.notify("ลืมปิดเเก๊ส"); 
  }
}
