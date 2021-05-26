#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
 
String apiKey = ""; // Enter your Write API key from ThingSpeak
const char *ssid = "";
const char *pass = "";
const char *server = "api.thingspeak.com";

const int AirValue = 710;   //air value sensor
const int WaterValue = 300;  //water value sensor
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent=0;
 
WiFiClient client;

void setup() {
  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(4000);
}  

void loop() 
{  
  soilMoistureValue = analogRead(SensorPin);
  Serial.println(soilMoistureValue);
  
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
 
 
if(soilmoisturepercent > 100)
{
  Serial.println("100 %");
  soilmoisturepercent = 100;
  delay(1800000);
}
 
 
else if(soilmoisturepercent <0)
{
  Serial.println("0 %");
  soilmoisturepercent = 0;
  delay(1800000);
}
 
else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
{
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  delay(1800000);
}
 
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(soilmoisturepercent);
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   
  }
    client.stop();
  
}
