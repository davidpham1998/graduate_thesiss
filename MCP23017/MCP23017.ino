#include <Wire.h>
#define MCP 0x20 // address MCP
byte sta, value;

byte Read()
{ Wire.beginTransmission(MCP);
  Wire.write(0x12);   // address port a
  Wire.endTransmission();
  Wire.requestFrom(MCP, 1);    // request 1 bytes from MCP23017 device
  return Wire.read();    // receive a byte as character
}

void setup()
{ Wire.begin();
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);

  Wire.beginTransmission(MCP);
  Wire.write(0x04);
  Wire.write(0xff);
  Wire.endTransmission();
}

void loop()
{ sta = digitalRead(2);
  if (sta == 0)
  {
  value = Read();
  Serial.println(value,BIN);
  delay(100);
    } 

}
