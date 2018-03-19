
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
#define ThingSpeak 0
#define IFTTT 1

const char* host = "api.thingspeak.com"; // Your domain  
const char* host2 ="maker.ifttt.com";

String ApiKey = "U87CBNFW57C1V1CC";
String IFTTTKEY="dCsrRRUCkeIzC2irwNBaGL";
String EVENTO="nodeMcu_temperature";
String path = "/update?key=" + ApiKey + "&field1=";  


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

const char* ssid = "SK_WiFiE4EF";
const char* pass = "1509011593";


char temperatureString[6];

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  DS18B20.begin();
   

}

float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

void loop() {

  float temperature = getTemperature();
  
  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);

  WiFiClient client;
  const int httpPort = 80;
  
  int key=IFTTT;
  
  if(key==ThingSpeak){

          if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
           }
                client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  }

  
  else if(key==IFTTT){
        
     if (!client.connect(host2, httpPort)) {
     Serial.println("connection failed");
     return;
     }
    Serial.println("Connected");
   
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += temperatureString;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += host2;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";

    client.print(toSend);
  }
 delay(5000);
  }


