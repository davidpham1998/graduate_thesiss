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

unsigned int sent(const uint64_t pipe, unsigned int num)
{
  if (millis() - now > 100) {
    radio.stopListening();
    now = millis();
    radio.openWritingPipe(pipe);
    radio.write(&num, sizeof(num));
  }
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  return num ;
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
  sent(slave1, set_hum);
  Serial.print("[SENT] ");     Serial.println(set_hum);
  //while (!radio.available());
  Serial.print("[RECV] ");     Serial.println(recieve(slave1));
  delay(1000);
  Serial.println("");

}
