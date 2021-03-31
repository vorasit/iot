
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "3fx1L_FPHueXrGwIzuoRvf7SVzWO1UH7";
char ssid[] = "B4-12_2G";
char pass[] = "12345678";

//const int pingPin = D1; //Tri รับค่า
//int inPin = D2; //echo ยิงออก
#define TRIGGERPIN D1   // ultrasonic
#define ECHOPIN D2      // ultrasonic
WidgetLCD lcd(V2);
BlynkTimer timer;

void setup() {
Serial.begin(9600);
 // Debug console
  Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  Blynk.begin(auth,ssid,pass);
  //Blynk.begin(auth, ssid, pass, "prakitblog.com", 8181);

  // Setup a function to be called every second
  
  timer.setInterval(2000L, sendUltrasonic);
}

void loop()
{
  Blynk.run();
  timer.run();
}


long microsecondsToCentimeters(long microseconds)
{
// ความเร็วเสียงในอากาศประมาณ 340 เมตร/วินาที หรือ 29 ไมโครวินาที/เซนติเมตร
// ระยะทางที่ส่งเสียงออกไปจนเสียงสะท้อนกลับมาสามารถใช้หาระยะทางของวัตถุได้
// เวลาที่ใช้คือ ระยะทางไปกลับ ดังนั้นระยะทางคือ ครึ่งหนึ่งของที่วัดได้
return microseconds / 29 / 2;
}

void sendUltrasonic()
{
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "Distance in cm");
  long duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println("Cm");
  lcd.print(7, 1, distance);
  if(distance<10){
    //Blynk.email("vorasit.pin@gmail.com","ถังขยะเต็มแล้ว","ถังขยะเต็มให้จัดเก็บด่วน");
    //Blynk.notify("มีการแจ้งเตือน -Bin is full");
    Blynk.notify("มีการแจ้งเตือน คุณอยู่ใกล้ผม คุณน่ารักเกินไป");
  }
  
}
