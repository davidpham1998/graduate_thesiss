#include <DHT.h>

#define misty 6       // phun sương
#define DHTPIN 2      // Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
#define DHTTYPE DHT11 //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
unsigned long nowDht;
unsigned int tThresh, hThresh;

byte t_current, dif, out, pre_tem;
bool flag = false;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600); // debug
  dht.begin();
  out = 150;
}

void loop()
{ tThresh = 28;
  t_current = dht.readTemperature();
  if (t_current >= tThresh) {
    if (pre_tem != t_current) {
      dif = t_current - tThresh;
      out = 150 + (dif * 8); // maximum outside temp: 45, 
      Serial.println(t_current);
      delay(50);
      Serial.print("out : ");
      Serial.println(out);
      pre_tem = t_current;
    }
    analogWrite(misty, out);
  }
  else analogWrite(misty, 0);
}
