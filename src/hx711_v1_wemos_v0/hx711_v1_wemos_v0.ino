#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h> 
#include "HX711.h"

#define pData D7
#define pClk D6
 
// Pin de datos y de reloj
byte pinData = pData;
byte pinClk = pClk;
 
HX711 bascula;

// ssid y pass de WiFI 
char* ssid = "WIFI";
char* password = "CLAVE";


// parámetros MQTT
const char* mqttServer = "industrial.api.ubidots.com";
int port = 1883;
char* subscribeTopic = "/v1.6/devices/panaldemo1/weight";
const char* mqtt_password = "";
const char* mqtt_user = "TOKEN";


WiFiClient espClient;
PubSubClient client(espClient);
char clientId[50];
DynamicJsonDocument doc(1024);
char* data = "";
int tiempo_monitoreo_peso = 1000;
// Parámetro para calibrar el peso y el sensor
float factor_calibracion = 25500.0; //Este valor del factor de calibración funciona para mi. El tuyo probablemente será diferente.
 
void setup() {

  // ** Puerto Serie***
  Serial.begin(115200); 

// ***** Configurando WiFi ************
  Serial.print("Conectando a WiFi ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("conectado");

  //***** Obtener direccion IP asignada *******
  IPAddress ip;
  ip = WiFi.localIP();
  Serial.print("Direccion IP: ");
  Serial.println(ip);

  //***** Configurando MQTT Client ************
  client.setServer(mqttServer, port); 
  
  //***** Configurando Báscula ******
  Serial.println("HX711 programa de calibracion");
  Serial.println("Quita cualquier peso de la bascula");
  Serial.println("Una vez empiece a mostrar informacion de medidas, coloca un peso conocido encima de la bascula");
  Serial.println("Ingrese el factor directamente en el input del monitor serial, este se irá mostrando para que tome nota y luego ajuste la variable correpondiente");
 
  // Iniciar sensor
  bascula.begin(pinData, pinClk);
 
  // Aplicar la calibración
  bascula.set_scale(); 
  bascula.tare();
 
  // Obtener una lectura de referencia
  long zero_factor = bascula.read_average();
  // Mostrar la primera desviación
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
   
}
 
void loop() {

  delay(tiempo_monitoreo_peso);
 
  // Aplicar calibración báscula
  bascula.set_scale(factor_calibracion);
  float weight = bascula.get_units() * 1000; 


  // Mostrar la información para ajustar el factor de calibración
  Serial.print("Leyendo: ");
  Serial.print(weight);
   
  Serial.println(); 
 
  // Obtener información desde el monitor serie
  if (Serial.available())
  {
    int numero = 0; // Variable para almacenar el número entero
    String texto = Serial.readStringUntil('\n'); // Lee el texto hasta el final de la línea
    factor_calibracion = texto.toInt(); // Convierte el texto a un número entero
   
  }

  Serial.print("factor de calibración es: ");
  Serial.println(factor_calibracion); // Imprime el número
 
   if (client.connected())
   { 
     doc["value"] = weight;
     String serializedJSON;
     serializeJson(doc, serializedJSON);
     client.publish(subscribeTopic,serializedJSON.c_str()); 
     Serial.println("publish succefully");
   }
   else
   {
     mqttReconnect();      
   }
   client.loop();
}


// Para reconección al servidor MQTT
void mqttReconnect(){
    while (!client.connected()){
        Serial.print("Intentando conectar al broker MQTT con cliente ");
        long r = random(1000);
        sprintf(clientId, "clientId-%ld", r);
        if (client.connect(clientId,mqtt_user, mqtt_password)){
            Serial.print(clientId);
            Serial.println(".......conectado");
            client.subscribe(subscribeTopic);
        }
        else{
            Serial.print("falló, rc=");
            Serial.print(client.state());
            Serial.println(".....Reintentando en 5 segundos");
            delay(5000);
        }
    }
}
