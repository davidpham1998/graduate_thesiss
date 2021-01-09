#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
void setup()
{ mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
}
void loop()
{
  if (mySerial.available())
  { digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);
    Serial.println(char(mySerial.read()));
  }
  //mySerial.print("nhan");
  //delay(1234);
}
