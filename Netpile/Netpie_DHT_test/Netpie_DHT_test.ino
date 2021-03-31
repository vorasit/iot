#include <MicroGear.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid     = "PIN_ITI24_2.4G";
const char* password = "0939025133";

#define APPID   "vorasit"
#define KEY     "J8mAjFSNzN8RlUz"
#define SECRET  "WeflVCITg2hcb7Kc0Q8Yub5Ra"

#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"

#define D4 2   // TXD1
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() 
{
     /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    dht.begin();
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

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("connected");

       float Humidity = dht.readHumidity();
       float Temp = dht.readTemperature();  // Read temperature as Celsius (the default)
       String data = "/" + String(Humidity) + "/" + String(Temp);
       char msg[128];
       data.toCharArray(msg,data.length());
       Serial.println(msg);    

       microgear.chat(TargetWeb , msg);
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(1500);
}
