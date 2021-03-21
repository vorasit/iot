#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "aVviPhZMMDlIyuiY-qiTZm1GIRcFsm-X";
char ssid[] = "AndroidAPDCC4";
char password[] = "wzcn9521";

 String tmp = "";
int count = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).


void setup()
{
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  // Debug console
  Serial.begin(115200);
  Serial.println("Starting...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

  // Setup Blynk
  Blynk.begin(auth, ssid, password);
  while (Blynk.connect() == false) 
  {
  }
  Blynk.syncAll();
  //Blynk.virtualWrite(V127, "Hello World");
  delay(5000);
  Blynk.syncVirtual(V127);
}
void loop()
{
  Blynk.run();
  count = count + 1;
  //Blynk.virtualWrite(V100, count);
  //delay(3000);
}
BLYNK_WRITE(V0) // V5 is the number of Virtual Pin
{
  int pinValue = param.asInt();
  digitalWrite(D1, !pinValue);
  Blynk.virtualWrite(V1, pinValue);
}

// restoring counter from server
BLYNK_WRITE(V100)
{
  //restoring int value
  count = param.asInt();
}

// restoring counter from server
BLYNK_WRITE(V127)
{
  //restoring int value
  tmp =  param.asString();
  Serial.println("V 127 data : " + tmp); 
}
