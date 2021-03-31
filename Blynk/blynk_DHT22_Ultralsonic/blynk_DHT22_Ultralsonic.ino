
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "aVviPhZMMDlIyuiY-qiTZm1GIRcFsm-X";
char ssid[] = "AndroidAPDCC4";
char pass[] = "wzcn9521";

#define TRIGGERPIN D5   // ultrasonic
#define ECHOPIN D6      // ultrasonic
#define DHTPIN D2       // What digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
WidgetLCD lcd(V2);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temp : ");
  Serial.print(t);
  Serial.println(" C");
  Serial.print("Humid : ");
  Serial.print(h);
  Serial.println(" %");

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  if(t>30){
    Blynk.email("vorasit.pin@gmail.com","อุณภูมิสูง","อุณภูมิสูงมากกก");
    Blynk.notify("มีการแจ้งเตือน - อุณภูมิสูงมากกก");
  }
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
    Blynk.email("vorasit.pin@gmail.com","ถังขยะเต็มแล้ว","ถังขยะเต็มให้จัดเก็บด่วน");
    Blynk.notify("มีการแจ้งเตือน -Bin is full");
  }
  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  Blynk.begin(auth,ssid,pass);
  //Blynk.begin(auth, ssid, pass, "prakitblog.com", 8181);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(3000L, sendSensor);
  timer.setInterval(2000L, sendUltrasonic);
}

void loop()
{
  Blynk.run();
  timer.run();
}
