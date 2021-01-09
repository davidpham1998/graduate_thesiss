#include <SPI.h>
#include <RF24.h>

const uint64_t slave1 = 0xE8E8F0F0E1DD; // địa chỉ phát
const uint64_t slave2 = 0xE8E8F0F0E1LL; // địa chỉ phát
RF24 radio(9, 10); //thay 10 thành 53 với mega
unsigned int rec;
long now = 0;
byte i, j;
byte set_hum = 12;
byte pre_hum = 1;

char strSetHum1 [] = "Hello Slave";
char strSetHum2 [] = "Hello world";

//String strSetHum  = "Hello Slave";

unsigned int recieve(uint64_t slaveAddr)
{ radio.openReadingPipe(1, slaveAddr);
  radio.startListening();
  if (radio.available())
  {
    radio.read(&rec, sizeof(rec));
    //Serial.println(rec);
  }
  return rec;
}

void sent(const uint64_t pipe, char *strSend, size_t ptrLen)
{
  if (millis() - now > 100) {
    radio.stopListening();
    now = millis();
    radio.openWritingPipe(pipe);
    radio.write(strSend, (ptrLen));
  }
//  radio.openReadingPipe(1, pipe);
//  radio.startListening();
//  return 0 ;
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(1, 1);
  radio.setDataRate(RF24_1MBPS); // data speed
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(10); // set channel

}

void loop() {
  sent(slave1, strSetHum1, sizeof(strSetHum1));
//  Serial.print("[SENT] ");     //Serial.println(strSetHum);
//  Serial.print("[SIZE] ");     Serial.println(sizeof(strSetHum1));
  delay(100);
  //while (!radio.available());
  Serial.print("[RECV1] ");     
  Serial.println(recieve(slave1));
  delay(1000);
  Serial.println("");

  sent(slave2, strSetHum2, sizeof(strSetHum2));
  delay(100);
  //while (!radio.available());
  Serial.print("[RECV2] ");     
  Serial.println(recieve(slave2));
  delay(1000);
  Serial.println("");

}
