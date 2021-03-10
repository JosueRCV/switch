//------------------------LIBRERIAS--------------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//------------------------DATOS--------------------------------------------------------------------
#define DER D1 // PIN DE SWITCH
#define IZQ D2 // PIN DE SWITCH
int Waiting_Time = 500;                  
const int LED = 2; //GPIO 2 corresponde al LED integrado de las placas NodeMCU - Wemos D1  
int b1;
int b2;
int key=0;
String post = "http://104.131.182.170:8000/post";// Enlace a la API


//----------------------CONEXION WIFI-------------------------------------------------

const char* ssid = "ARRIS-F782";
const char* password = "C85D6D6196489379";

//-------GLOBALES-------------------

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


//-------------------------------------------FUNCIONES---------------------------------------------------------------------------------------------------------

void setup() {
  pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D1,INPUT_PULLUP); // Derecha
  pinMode(D2,INPUT_PULLUP); // Izquierda
  digitalWrite(LED, HIGH); 
  Serial.begin(115200);
  setup_wifi();

}


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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
}

void loop() {    
  estado();
}

void estado() {
  // put your main code here, to run repeatedly:

/*DIP2*/
  if(digitalRead(IZQ)==LOW)
  {b1=1;}
  else if(digitalRead(DER)==LOW)
  {b1=2;}
  else if((digitalRead(IZQ)==HIGH)&&(digitalRead(DER)==HIGH))
  {b1=3;}
  if(b1!=b2)
  {
  if((digitalRead(IZQ)==HIGH)&&(digitalRead(DER)==HIGH))
  {
      b2=b1;
      Serial.print("APAGADO INTERRUPTOR");
      Serial.println();
      String estado = "Apagado";
      estado.toCharArray(msg,50);
      postinterruptor(estado);
      delay(200);
   }
   else
   {
      if(digitalRead(DER))
    {
       b2=b1;
       Serial.print("DERECHA");
       Serial.println();
       String estado = "Derecha";
       estado.toCharArray(msg,50);
       postinterruptor(estado);
       delay(200);
       
    
    }
      if(digitalRead(IZQ)&&not(digitalRead(DER)))
    {
      b2=b1;
       Serial.print("IZQUIERDA");
       Serial.println();
       String estado = "Izquierda";
       estado.toCharArray(msg,50);
       postinterruptor(estado);
       delay(200);
    
    }
    }
  
  }
}


void postinterruptor(String estado ) {
  HTTPClient http;
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //root["_key"] = key;
  //key=key+1;
  root["estado"] = estado;
  /* Ejemplo si quieres más atributos
  root["sensor"] = "gps";
  root["time"] = 1351824120;*/
  root.printTo(json);
  
  Serial.println(""); // salto de linea para http.writeToStream(&Serial);
  
  http.begin(post); //URL del microservicio
  http.addHeader("Content-Type", "application/json");
  http.POST(json); 
  http.writeToStream(&Serial);
  http.end();

  //delay(1000);
}
