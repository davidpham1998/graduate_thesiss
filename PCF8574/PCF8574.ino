#include <Wire.h>
#define PCF_ADDr 0x38 // address PCF_ADDr8574A
#define LED 8
byte value, sta, pre ;

void setup()
{
  Wire.begin(15, 14);
  //  Wire.begin();
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(16, INPUT);
  //  pinMode(2, INPUT);
  pinMode(33, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(33, 0);
  digitalWrite(4, 0);
  pre = 0;
  //  attachInterrupt(digitalPinToInterrupt(2),reading, FALLING);
}

void loop() {
  //  sta = digitalRead(16);
  if (digitalRead(16) == 0)
  {
    reading();
  }
}

void reading() {
  Wire.beginTransmission(PCF_ADDr_ADDr);
  Wire.write(0XFF);
  Wire.endTransmission();
  Wire.requestFrom(PCF_ADDr, 1);    // request 1 bytes from MCP23017 device
  Wire.beginTransmission(PCF_ADDr);
  Wire.write(0xFF);
  Wire.endTransmission();
  Wire.requestFrom(PCF_ADDr, 1);

  while (Wire.available())
  {
    value = Wire.read();
    Serial.println(value, BIN);
    digitalWrite(33, 1);
    if (value - pre)
    {
      switch (value)
      {
        // low
        case 0b11111111:
//          webSocket.sendTXT("[{\"EVENT\":\"wtlv\",\"uId\":\"" + finalUID + "\",\"gId\":\"" + WiFi.macAddress() + "\",\"val\":30}]");
          digitalWrite(4, 0);
          break;

        // medium
        case 0b01111111:
//          webSocket.sendTXT("[{\"EVENT\":\"wtlv\",\"uId\":\"" + finalUID + "\",\"gId\":\"" + WiFi.macAddress() + "\",\"val\":60}]");
          digitalWrite(4, 1);
          break;

        // high
        case 0b00111111:
//          webSocket.sendTXT("[{\"EVENT\":\"wtlv\",\"uId\":\"" + finalUID + "\",\"gId\":\"" + WiFi.macAddress() + "\",\"val\":90}]");
          digitalWrite(4, 0);
          break;
      }
      pre = value;
    }
  }
  delay(100);
}
