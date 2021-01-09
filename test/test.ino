#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

char ssid[] = "david";
char pass[] = "chitam123";
String apikey = " mcAFD98WOXOTlBVHfzs2AuJ7kQweKd1n";
String citycode = "354305";
char sever[] ="dataservice.accuweather.com";
unsigned long timex ;
char txt[2000];
String line;
int i;

WiFiClient client;

void getweather()
{ Serial.println("connect to >>>"+ String(sever));
  if(client.connect(sever,80))
  { Serial.println("connect sucess");
    client.print("GET http://dataservice.accuweather.com/forecasts/v1/daily/1day/" + citycode + "?apikey=" + apikey +" HTTP/1.0\r\n" );
    client.println("connection: close");
    client.println();
    delay(10);
    }
  else 
  {Serial.println("connect fail");
    }
  while(client.connected())
  { line = client.readStringUntil('\n');
    fifter();
    }
  }

void fifter()
{ DynamicJsonBuffer jsonBuffer(line.length()+1);
  line.toCharArray(txt,line.length()+1);
  JsonObject & root = jsonBuffer.parseObject(txt);
  float tempmax = root["DailyForecasts"][0]["Temperature"]["Maximum"]["Value"];
  int   tempmin = root["DailyForecasts"][0]["Temperature"]["Minimum"]["Value"];
  String wea    = root["DailyForecasts"][0]["Day"]["IconPharse"];
  //Serial.print(txt);
  if(tempmax != 0)
  { Serial.println("");
    Serial.println("temp_max: " + String(tempmax)+);
    Serial.println("");
    Serial.println("temp_min: " + String(tempmin)+ " C");
    Serial.print("\t weather: " + String(wea));
    Serial.println("\n");
   }
  }

void setup() {
  Serial.begin(9600);
  Serial.print("connecting to");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED)
  { delay(10);
    Serial.print(".");
    }
  Serial.println("");
  Serial.print("WiFi connected\t IP adress: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if(millis()- timex > 1000 ){ // after 1s appear a time
  getweather();
  timex = millis();
  }
}
