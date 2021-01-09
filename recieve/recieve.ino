#include <SPI.h>
#include <RF24.h>
 
const uint64_t pipe = 0xE8E8F0F0E1DD; // địa chỉ phát
RF24 radio(9,10); 
byte msg[3];
 
void setup(){
Serial.begin(9600);
radio.begin(); 
radio.setAutoAck(1); 
radio.setDataRate(RF24_1MBPS); // Tốc độ dữ liệu
radio.setChannel(10); // Đặt kênh
radio.openReadingPipe(1,pipe); 
radio.startListening(); 
}
 
void loop(){
if (radio.available()){
while (radio.available()){
radio.read(&msg, sizeof(msg));
Serial.println(msg[0]);
}
delay(100);
}
}
