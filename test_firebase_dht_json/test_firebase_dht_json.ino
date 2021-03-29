#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//#include <FirebaseArduino.h>
#include <DHT.h>
#include <time.h>
//#include <FirebaseJson.h>
#define FIREBASE_HOST "kmutnbline-gpvrvc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "*************************"
// Config connect WiFi
#define WIFI_SSID "PIN_ITI24_2.4G"
#define WIFI_PASSWORD "0939025133"
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int timezone = 7;
char ntp_server1[20] = "ntp.ku.ac.th";
char ntp_server2[20] = "fw.eng.ku.ac.th";
char ntp_server3[20] = "time.uni.net.th";
int dst = 0;
//Define FirebaseESP8266 data object
FirebaseData fbdo;

FirebaseJson json;
FirebaseJson json1; //Temperature
int i=0;

String NowString() {
 time_t now = time(nullptr);
 struct tm* newtime = localtime(&now);

 String tmpNow = "";
 tmpNow += String(newtime->tm_year + 1900);
 tmpNow += "-";
 tmpNow += String(newtime->tm_mon + 1);
 tmpNow += "-";
 tmpNow += String(newtime->tm_mday);
 tmpNow += " ";
 tmpNow += String(newtime->tm_hour);
 tmpNow += ":";
 tmpNow += String(newtime->tm_min);
 tmpNow += ":";
 tmpNow += String(newtime->tm_sec);
 return tmpNow;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  fbdo.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);
  
configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
 Serial.println("Waiting for time");
 while (!time(nullptr)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
Serial.println("Now: " + NowString());
  
String path = "/DHTData";

  Serial.println("------------------------------------");
  Serial.println("Set double test...");
  dht.begin();

  
}

void loop() {
  // Read temp & Humidity for DHT22
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 String path = "/DHTData";

 if (isnan(h) || isnan(t)) {
 Serial.println("Failed to read from DHT sensor!");
 delay(1000);
 return;
 }
   Serial.print("Humidity: ");
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Temperature: ");
   Serial.print(t);
   Serial.print(" *C ");
   Serial.println();
   delay(1000);

 //Humidity
   json.clear().add(NowString(), h);

    //Also can use Firebase.push instead of Firebase.pushJSON
    //Json string is not support in v 2.6.0 and later, only FirebaseJson object is supported.
    if (Firebase.pushJSON(fbdo, path + "Humidity:", json))
    {
      Serial.println("PASSED Humidity");
      delay(1000);
    }
    else
    {
      Serial.println("FAILED Humidity");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    //Temperature
   json1.clear().add(NowString(), t);

    //Also can use Firebase.push instead of Firebase.pushJSON
    //Json string is not support in v 2.6.0 and later, only FirebaseJson object is supported.
    if (Firebase.pushJSON(fbdo, path + "Temperature:", json1))
    {
      Serial.println("PASSED Temperature");
      delay(1000);
    }
    else
    {
      Serial.println("FAILED Temperature");
      Serial.println("REASON: " + fbdo.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
 
}
