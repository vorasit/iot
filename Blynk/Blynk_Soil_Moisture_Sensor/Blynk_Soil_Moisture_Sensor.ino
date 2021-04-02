#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "3fx1L_FPHueXrGwIzuoRvf7SVzWO1UH7";
char ssid[] = "B4-12_2G";
char pass[] = "12345678";

WidgetLCD lcd(V2);
BlynkTimer timer;

int ledPin = D0;
int ledPin3 = D1;
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
void setup() {
 
 // Debug console
  Serial.begin(9600);

  Blynk.begin(auth,ssid,pass);
  //Blynk.begin(auth, ssid, pass, "prakitblog.com", 8181);

  // Setup a function to be called every second
  
  timer.setInterval(2000L, soil);
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
    Blynk.notify("แห้ง");
  }
  else {
    //ชื้น
    lcd.print(7, 1, val);
    Blynk.notify("ชื้น");
  }
  delay(1000);
}
