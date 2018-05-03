#include <ArduinoJson.h>
#include <Arduino.h>


void setup() {
  Serial.begin(115200); /* Define baud rate for serial communication */
}

void loop() {
  String ledStatus;
  String servoStatus;

  if (Serial.available())  /* If data is available on serial port */
  {
    String phrase;    
    char data_received;
    data_received = Serial.read();  /* Data received from bluetooth */
    phrase = String(phrase + data_received);
    Serial.print(phrase);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& jsonResponse = jsonBuffer.parseObject(phrase);  
    ledStatus = jsonResponse["Light"].asString();;
    servoStatus = jsonResponse["Stove"].asString();; 

  
  
  }
}