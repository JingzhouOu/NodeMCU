#include <ArduinoJson.h>
#include <Servo.h>

#define PIN_SERVO 5
#define PIN_SERVO2 4
Servo myservo;
Servo myservo2;

void setup() {
  Serial.begin(115200); /* Define baud rate for serial communication */
}

void loop() {
  String ledStatus;
  String servoStatus;


  /*Data received from WiFi*/
  //  if (Serial.available() > 0)     // Send data only when you receive data:
  //  {
  //    String data_received;
  //    data_received = Serial.read();  /* Data received from bluetooth */
  //    Serial.print(data_received);
  //    StaticJsonBuffer<200> jsonBuffer;
  //    JsonObject& jsonResponse = jsonBuffer.parseObject(data_received);
  //    ledStatus = jsonResponse["Light"].asString();;
  //    servoStatus = jsonResponse["Stove"].asString();;
  //    if (servoStatus == 'off') {
  //      myservo.attach(PIN_SERVO);
  //      myservo2.attach(PIN_SERVO2);
  //      myservo.write(0);
  //      myservo2.write(0);
  //      delay(1000);
  //      myservo.detach();
  //      myservo2.detach();
  //    }

  /* For testing with phone connection*/
  if (Serial.available())  /* If data is available on serial port */
  {
    char data_received;
    data_received = Serial.read();  /* Data received from bluetooth */
    Serial.print(data_received);
    if (data_received == '1') {
      myservo.attach(PIN_SERVO);
      myservo2.attach(PIN_SERVO2);
      myservo.write(0);
      myservo2.write(0);
      delay(1000);
      myservo.detach();
      myservo2.detach();
    }
  }

}