#include <ArduinoJson.h>


String ledStatus;
String servoStatus;

int LED = D4;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600); /* Define baud rate for serial communication */
}

void loop() {

  if (Serial.available())  /* If data is available on serial port */
  {
    char data_received;
    data_received = Serial.read();  /* Data received from bluetooth */
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& jsonResponse = jsonBuffer.parseObject(data_received);
    ledStatus = jsonResponse["Light"].asString();;
    servoStatus = jsonResponse["Stove"].asString();;

    if (ledStatus == "On")
    {
      digitalWrite(LED, HIGH);
      Serial.write("LED turned ON\n");
    }
    else if (ledStatus == "Off")
    {
      digitalWrite(LED, LOW);
      Serial.write("LED turned OFF\n");
    }
    else
    {
      Serial.write("Select either 1 or 2");
    }
  }
}