#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "fauxmoESP.h"
#include <ArduinoJson.h>
#include <Adafruit_MLX90614.h>
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define LED  D5
#define BUZZER  D7
#define ALARM "{'Light':'off','Stove':'off'}"

const char* WIFI_SSID = "Demian";  //wifi ssid
const char* WIFI_PASS = "ustb1994";  //wifi password 

void httpControl();
void pinControl();

String ledStatus;
String servoStatus;
HTTPClient http;  //Declare an object of class HTTPClient

 

//----- Setup function. ------------------------
void setup() {             
  wifiSetup();
  mlx.begin();  
  pinMode(LED, OUTPUT);
}

//----- Loop routine. --------------------------
void loop() {
  httpControl();
}

void wifiSetup() 
{
   Serial.begin(115200);
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
}
/*
 * Request for API and parson response to JSON
 */
void httpControl()
{
  double temp = mlx.readObjectTempC();
  String tempurl = "http://39.106.107.244:8080/springTest/led/v1?id=3&name=";
  tempurl = tempurl + temp;
  if (temp > 30) {
    Serial.print('1');
    http.begin(tempurl);  //Specify request destination
    int httpCode = http.GET();  //Send the request
    
   tone(BUZZER, 1000); // Send 1KHz sound signal...
   digitalWrite(LED, HIGH);
  
   delay(1000);        // ...for 1 sec
   noTone(BUZZER);     // Stop sound...
   digitalWrite(LED, LOW);
   delay(1000);        // ...for 1sec
  }
 

  http.begin("http://39.106.107.244:8080/springTest/user/id/1");  //Specify request destination
  int httpCode = http.GET();  //Send the request
  if (httpCode > 0) 
  { //Check the returning code
    String response = http.getString();   //Get the request response payload
    Serial.println(response);  //Print the response payload
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& jsonResponse = jsonBuffer.parseObject(response);  
    ledStatus = jsonResponse["Light"].asString();;
    servoStatus = jsonResponse["Stove"].asString();; 
    if (servoStatus == "off") {
      Serial.print('1');
    }
  }
 
  http.end();   //Close connection
  delay(200);
 
}
