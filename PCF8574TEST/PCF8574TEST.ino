#include <Wire.h>
#define PCF 0x38 // address PCF8574A
#define LED 8
byte value, sta, pre ;

void setup()
{
  //Wire.begin(15, 14);
  Wire.begin();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  //pinMode(16, INPUT);
  pinMode(2, INPUT);
  //pinMode(33, OUTPUT);
  //digitalWrite(33, 1);
  pre = 0;
  //  attachInterrupt(digitalPinToInterrupt(2),reading, FALLING);
}

void loop() {
  sta = digitalRead(2);
  if (sta == 0)
  {
    reading();
  }
}

void reading() {
  Wire.beginTransmission(PCF);
  Wire.write(0XFF);
  Wire.endTransmission();
  Wire.requestFrom(PCF, 1);    // request 1 bytes from MCP23017 device
  while (Wire.available())
  {
    value =  Wire.read();
    if (value != pre) {
      switch (value) {
        case 0b11111111 :
          Serial.println(value, BIN);
          break ;

        case 0b01111111 :
          Serial.println(value, BIN);
          break ;

        case 0b00111111 :
          Serial.println(value, BIN);
          break ;
      }
      pre = value;
    }
  }
  delay(100);
}
