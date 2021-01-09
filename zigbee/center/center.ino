#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial mySerial(2, 3); // RX, TX
StaticJsonBuffer<200> jsonBuffer;
String myId = "123";

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  String strSensor = "{\"humid\":" + String(rand(100)) + ",\"temp\":" + String(rand(30)) + ",\"id\":" + myId + "}";

  //mySerial.print(strSensor);
  delay(2000);
}
