#include <SPI.h>
#include <DHT.h>
#include <RF24.h>

const uint64_t ID = 11; // địa chỉ phát
RF24 radio(9,10); //thay 10 thành 53 với mega
const int spray = 3; // make the misty
const int water = 4; // water trees
const int DHTPIN = 2; //read data of DHT11 from pin 2
const int DHTTYPE = DHT11; //declaring sensor(DHT11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);
long total = 1;
unsigned long now = 0;
byte id, set_temp ,set_hum;

//char rec; // recieve variable
char rec[20]; // recieve variable
char recieve()
{

  radio.openReadingPipe(1, ID);
  radio.startListening();
  if (radio.available())
  {
      radio.read(&rec, sizeof(rec));
    //radio.read(rec, 20);
    while(!radio.available());      // wait the next data coming
    //Serial.println(rec);
  }
  //  return rec;
}

void sent(unsigned int num)
{
  if (millis() - now > 100) {
    radio.stopListening();
    now = millis();
    radio.openWritingPipe(ID);
    radio.write(&num, sizeof(num));
  }
  radio.openReadingPipe(1, ID);
  radio.startListening();
//  return 0 ;
}


void confirm()
{ 
  if((total/10000) == ID){ 
  set_temp = (total%10000)/100;
  set_hum = total%100;
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin(); // starting sensor

  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(1, 1);
  radio.setDataRate(RF24_1MBPS); // data speed
  radio.setPALevel(RF24_PA_LOW);
//  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(10); // set channel

  pinMode(spray, OUTPUT);
  pinMode(water, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  recieve();
  total = atol(rec);
  Serial.println(total);
  delay(10);
  confirm();
  Serial.print("hum: ");
  Serial.println(set_hum);
  Serial.print("temp: ");
  Serial.println(set_temp);
  Serial.println();
  delay(50);
  sent(134);
  delay(50);
}
