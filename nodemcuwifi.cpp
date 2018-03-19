#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "fauxmoESP.h"
#include <Servo.h>
#include <ArduinoJson.h>
#include <Adafruit_MLX90614.h>
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define MOTOR D4
#define LED  D3
#define STOVE D6

Servo servoMain;  // create servo object to control a servo 
const char* WIFI_SSID = "Demian";  //wifi ssid
const char* WIFI_PASS = "ustb1994";  //wifi password 

void servoControl();
void httpControl();
void pinControl();

String ledStatus;
String servoStatus;
HTTPClient http;  //Declare an object of class HTTPClient

 

//----- Setup function. ------------------------
void setup() {             
  servoMain.attach(MOTOR); // servo on digital pin D4
  servoMain.write(0);  // Turn Servo Left to 0 degrees
  wifiSetup();
  mlx.begin();  
  pinMode(STOVE, OUTPUT);
  pinMode(LED, OUTPUT);
}

//----- Loop routine. --------------------------
void loop() {
  httpControl();
  pinControl();
  
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
    http.begin(tempurl);  //Specify request destination
    int httpCode = http.GET();  //Send the request   
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
  }
 
  http.end();   //Close connection
  delay(200);
 
}
/*
 * Control pins according to response data
 */
void pinControl()
{
  if (ledStatus == "on")
    {
      digitalWrite(LED, HIGH);  
    }
    else if (ledStatus == "off")
    {
      digitalWrite(LED, LOW);
    }

    if (servoStatus == "on")
    {
      if (servoMain.read() != 90) 
      {
        servoMain.write(90);  // Turn Servo Left to 45 degrees
      }       
    }
    else if (servoStatus == "off")
    {
      if (servoMain.read() != 0) 
      {
        servoMain.write(0);  // Turn Servo Left to 45 degrees
      }         
    }
}

void servoControl()
{
   servoMain.write(45);  // Turn Servo Left to 45 degrees
   delay(1000);          // Wait 1 second
   servoMain.write(0);   // Turn Servo Left to 0 degrees
   delay(1000);          // Wait 1 second
    // Turn Servo back to center position (90 degrees)
   delay(1000);          // Wait 1 second
   servoMain.write(135); // Turn Servo Right to 135 degrees
   delay(1000);          // Wait 1 second
   servoMain.write(180); // Turn Servo Right to 180 degrees
   delay(1000);          // Wait 1 second
   servoMain.write(90);  // Turn Servo back to center position (90 degrees)
   delay(1000);          // Wait 1 second   
}