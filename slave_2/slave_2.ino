#include <SPI.h>
#include <DHT.h>
#include <RF24.h>

const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ phát
RF24 radio(9, 10); //thay 10 thành 53 với mega
const int spray = 3; // make the misty
const int water = 4; // water trees
const int DHTPIN = 2; //read data of DHT11 from pin 2
const int DHTTYPE = DHT11; //declaring sensor(DHT11 or DHT22)
DHT dht(DHTPIN, DHTTYPE);

byte rec; // recieve variable
long now = 0; 
byte set_hum; // setpoint for PID
int temp, air_hum, ground_hum; 
int  e_present, e_sum, e_delta, e_previous,duty;

byte out_pid(byte Kp, byte Ki, byte Kd ){
  e_present =  set_hum   - air_hum;       // current diferrent
  e_sum     =  e_present + e_previous;    // total differrent
  e_delta   =  e_present - e_previous;    // độ biến thiên của sai biệt hiện tại và trước đó
  e_present =  e_previous;
  duty      =  duty + Kp*e_present + Ki*e_sum + Kd*e_delta; // formula of PID
  return duty;
}

byte recieve()
{ radio.openReadingPipe(1, pipe);
  radio.startListening();
  if (radio.available())
  {
    radio.read(&rec, sizeof(rec));
    //while(!radio.available());      // wait the next data coming
    //Serial.println(rec);
  }
  return rec;
}

byte sent(unsigned int num)
{
  if (millis() - now > 100) {    // each 100ms send a time
    radio.stopListening();
    now = millis();
    radio.openWritingPipe(pipe); // mở kênh
    radio.write(&num, sizeof(num));
  }
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  return num ;
}

void setup() {
  Serial.begin(9600);
  dht.begin(); // starting sensor

  radio.begin(); 
  radio.setAutoAck(1);
  radio.setRetries(1, 1);
  radio.setDataRate(RF24_1MBPS); // data speed
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(10); // set channel

  pinMode(spray, OUTPUT);
  pinMode(water, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  set_hum = recieve();
  Serial.println(set_hum);
  while (!radio.available()); // wait the next data coming
  sent(set_hum);

  air_hum = dht.readHumidity(); //Reading HUMIDLITY
  temp = dht.readTemperature(); //Reading TEMPERATURe

  ground_hum = analogRead(A0);
  if (ground_hum > 550) {      
    digitalWrite(water, 1);
    delay(100);
  }

  analogWrite(spray,out_pid(5,1,0.01)); //  out_pid(a,b,c) tuong ung a=Kp ; b=Ki ; c=Kd
}
