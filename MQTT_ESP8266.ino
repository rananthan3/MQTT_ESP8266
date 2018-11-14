#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>
 


unsigned int Rs = 150000;
double Vcc = 3.3;
const char* ssid = "Cisco51413";
const char* password = "homeuser";
const char* mqtt_server = "192.168.1.73";
 
WiFiClient espClient;
PubSubClient client(espClient);


void setup()
{
  
 Serial.begin(9600); 
 delay(100);

 Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
 client.setServer(mqtt_server, 1883);

 client.connect("ESP8266 Client");
 
 //if (client.connect("ESP8266 Client")) {
  //Serial.println("connected");
  // ... and subscribe to topic
  //client.publish("temp", "Temperature: test");  
 //} else {
  //Serial.print("failed, rc=");
  //Serial.print(client.state());
 //}

 
}
 
void loop()
{
  
float temp = Thermister(AnalogRead());
float temp_f = temp*1.8 +32;

char result_celsius[8]; 
dtostrf(temp, 6, 2, result_celsius);

char result_fahren[8]; 
dtostrf(temp_f, 6, 2, result_fahren);


const char * a = "Temperature (°C): ";
const char * b = "Temperature (°F): ";


char str_c[30];
char str_f[30];

strcpy(str_c, a);
strcat(str_c, result_celsius);

strcpy(str_f,b);
strcat(str_f,result_fahren);


client.publish("tempC", str_c); 
client.publish("tempF", str_f);   

delay(100);

ESP.deepSleep(30e6); 

}



int AnalogRead() {
  int val = 0;
  for(int i = 0; i < 20; i++) {
    val += analogRead(A0);
    delay(1);
  }

  val = val / 20;
  return val;
}

double Thermister(int val) {
  double V_NTC = (double)val / 1024;
  double R_NTC = (Rs * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  //Temp = Temp - 273.15;       
   Temp = Temp - 303.01;         
  return Temp;

}
