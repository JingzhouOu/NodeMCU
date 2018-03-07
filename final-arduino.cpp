#include <ArduinoJson.h>

void setup()
{
  Serial.begin(9600);   //Sets the baud for serial data transmission
  pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
}
void loop()
{
  if (Serial.available() > 0) 
  {
    String data = Serial.readStringUntil('}');  /* Data received from bluetooth */
    StaticJsonBuffer<200> jsonBuffer;
    data = data + "}";
    Serial.print(data);          //Print Value inside data in Serial monitor
    JsonObject& jsonResponse = jsonBuffer.parseObject(data);
    String ledStatus = jsonResponse["Light"].asString();;
    String servoStatus = jsonResponse["Stove"].asString();;


    if (ledStatus == "on")
    {
      digitalWrite(13, HIGH);
    }
    else if (ledStatus == "off")
    {
      digitalWrite(13, LOW);
    }


  }
}