#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "DHT.h"

#define motor   7
#define misty   6 //  spraying the misty  
#define DHTPIN  2
#define DHTTYPE DHT11 //declar sensor
//String mode_gnd;
unsigned long nowDht ;
unsigned int tThresh, hThresh, request;
bool sta;

int h_current, dif, out;
bool flat = false;

DHT dht(DHTPIN, DHTTYPE);

void blink3times();

String deviceId = "1A";
SoftwareSerial mySer(2, 3); // RX TX
StaticJsonDocument<200> docParser;

//////// get average ground value
int avggnd(byte n) {
  unsigned int sum ;
  for (byte i = 0; i < n; i++)
  { sum  +=  analogRead(A0);
  }
  return (sum / n);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  mySer.begin(9600); // zigbee
  Serial.begin(9600); // debug
  dht.begin();

  while (!Serial || !mySer);
  if (EEPROM.read(0) == 0)
    Serial.println("This is BLANK NEW device");
  else if (EEPROM.read(0) == 1)
    Serial.println("This device has been registered");

  while (!EEPROM.read(0)) {
    // this is new device >> waiting for pairing via zigbee
    if (mySer.available()) {
      blink3times();

      String strRecv = mySer.readStringUntil('\n');
      Serial.println(strRecv);
      DeserializationError error = deserializeJson(docParser, strRecv);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());

        blink3times();
        // request master to resend error
        mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"initErr\"}");
      }
      else if (docParser["id"] == deviceId) {
        Serial.print("[Success] complete pairing new device");
        mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"initOK\"}");
        EEPROM.write(0, 1);
        blink3times();
      }
    }
  }
  nowDht = millis();
  out = 255;
}

void loop() {
  humidground();
  if (mySer.available()) {
    digitalWrite(LED_BUILTIN, 1);
    delay(20);
    digitalWrite(LED_BUILTIN, 0);
    String strRecv = mySer.readStringUntil('\n');
    Serial.println(strRecv);
    DeserializationError error = deserializeJson(docParser, strRecv);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    /////// compare id ////////
    if (docParser["deviceId"] == deviceId) {
      String temp  = docParser["temp"];
      String humid = docParser["humid"];
      String sum   = docParser["sum"];
      String mode_gnd = docParser["mode"];

      if (sum == (temp + humid))
      {
        tThresh = temp.toInt();
        hThresh = humid.toInt();
        request = mode_gnd.toInt();
        Serial.println(tThresh);

        Serial.print("Temp: ");
        Serial.println(temp);
        Serial.print("Humid: ");
        Serial.println(humid);
      }
    }

    //////// send a signal each a second
    if (millis() - nowDht > 2000) {
      String hum = String(dht.readHumidity());
      String tem = String(dht.readTemperature());
      String gnd = String(avggnd(10));
      String tus = String(sta); // motor status

      mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"temp\",\"value\":\""   + tem + "\"}");
      delay(50);
      mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"humid\",\"value\":\""  + hum + "\"}");
      delay(50);
      mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"ground\",\"value\":\"" + gnd + "\"}");
      delay(50);
      mySer.println("{\"id\":\"" + deviceId + "\",\"ev\":\"status\",\"value\":\"" + tus + "\"}");
      nowDht = millis();
    }
  }
  PID();
}

void blink3times() {
  digitalWrite(LED_BUILTIN, 1);
  delay(20);
  digitalWrite(LED_BUILTIN, 0);
  delay(20);
  digitalWrite(LED_BUILTIN, 1);
  delay(20);
  digitalWrite(LED_BUILTIN, 0);
}

void PID()
{ h_current = dht.readHumidity();
  if (flat == false)
  { analogWrite(misty, 255);
    if (h_current == hThresh) {
      dif = hThresh - h_current;
      out = out + dif;
      delay(10);
      analogWrite(misty, out);
      flat = true ;
    }
  }
  else if (flat == true)
  {
    dif = hThresh - h_current;
    out = out + dif;
    delay(10);
    analogWrite(misty, out);
  }
}

void humidground() {
  if (avggnd > 850 && request == 0)
  {
    digitalWrite(motor, 1);
    delay(10);
    sta = 1;
  }
  else if (avggnd < 300 && request == 0) {
    digitalWrite(motor, 0);
    delay(10);
    sta = 0;
  }

  if (request == 1)     // sta variable is unnecessary. because this request get from the server
  { digitalWrite(motor, 1);
    delay(10);
  }
  else {
    digitalWrite(motor, 0);
    delay(10);
  }
}
