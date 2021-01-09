#include <SPI.h>
#include <RF24.h>

const uint64_t slave1 = 11; // địa chỉ phát
const uint64_t slave2 = 12; // địa chỉ phát
RF24 radio(2, 15); //thay 10 thành 53 với mega
unsigned int rec;
unsigned int buf[] = {};
long now = 0;
byte i, j;
byte set_hum = 12;
byte pre_hum = 1;



char strSetHum1 [] = "112380";
char strSetHum2 [] = "123050";

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
  radio.setPALevel(RF24_PA_LOW);
  //radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(10); // set channel

}

void loop() {
  sent(slave1, strSetHum1, sizeof(strSetHum1));
//  Serial.print("[SENT] ");     //Serial.println(strSetHum);
//  Serial.print("[SIZE] ");     Serial.println(sizeof(strSetHum1));
  delay(100);
  Serial.print("[RECV1] ");     
  Serial.println(recieve(slave1));
  Serial.println("");
  
  delay(100);
  sent(slave2, strSetHum2, sizeof(strSetHum2));
  delay(1000);
  //while (!radio.available());     
   Serial.print("[RECV2] ");
   Serial.println(recieve(slave2));
   delay(100);
   Serial.println("");

//    sent(slave1, strSetHum1, sizeof(strSetHum1));
//    delay(500);
//    sent(slave2, strSetHum2, sizeof(strSetHum2));
//    delay(500);
//    for(i=0;i<2;i++){
//      buf[i] = recieve(slave1);
//      delay(50);
//      if(buf[i] == buf[i-1])
//      { delay(50);
//        buf[i] = recieve(slave2);
//        Serial.print("[RECV2] ");
//        Serial.println(buf[i]); 
//        }
//     delay(50);
//     Serial.print("[RECV1] ");
//     Serial.println(buf[i]);
//    }
//    

}
