#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "config.h"

// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

int freq = 2000;
int channel = 0;
int resolution = 8;

// Inicializando LittleFS
void initFS() {
 // Iniciamos  SPIFFS
  if(!SPIFFS.begin())
     { Serial.println("ha ocurrido un error al montar SPIFFS");
       return; }
}

// Inicializando WiFi
void initWiFi() {
// conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      // Esperamos un segundo
      Serial.println("Conectando a la red WiFi..");
    }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());
}
String getRSSI(){
  return String(WiFi.RSSI());
}
String processor(const String& var){
    Serial.print(var+" : ");
    if (var == "IP"){
      return String(WiFi.localIP().toString().c_str());
    }
    else if (var == "HOSTNAME"){
      return String(WiFi.getHostname());
    }
    else if (var == "STATUS"){
      return String(WiFi.status());
    }
    else if (var == "SSID"){
      return String(WiFi.SSID().c_str());
    }
    else if (var == "PSK"){
      return String(WiFi.psk().c_str());
    }
    else if (var == "BSSID"){
      return String(WiFi.BSSIDstr().c_str());
    }
    else if (var == "RSSI"){
      return String(WiFi.RSSI());
    }
}

void setup() { 

  Serial.begin(115200); // Set serial out if we want debugging
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(18, channel);
  initWiFi();
  initFS();

   // Ruta para cargar el archivo index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });


  server.serveStatic("/", SPIFFS, "/");

  // Ruta para poner el GPIO alto
  server.on("/RSSI", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",getRSSI().c_str());
  });

  //-----------------------------------------------------------------
  // Teclas del piano
  // Tecla 1
  server.on("/TECLA1", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 1 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 261);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 2
  server.on("/TECLA2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 2 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 277);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 3
  server.on("/TECLA3", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 3 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 293);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 4
  server.on("/TECLA4", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 4 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 311);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 5
  server.on("/TECLA5", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 5 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 329);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 6
  server.on("/TECLA6", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 6 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 349);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 7
  server.on("/TECLA7", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 7 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 369);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 8
  server.on("/TECLA8", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 8 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 392);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 9
  server.on("/TECLA9", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 9 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 415);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 10
  server.on("/TECLA10", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 10 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 440);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 11
  server.on("/TECLA11", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 11 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 466);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 12
  server.on("/TECLA12", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 12 presionada");
    ledcWrite(channel, 125);
    ledcWriteTone(channel, 493);
    delay(1000);
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  //-----------------------------------------------------------------

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
} 

void loop() {

  Serial.print("Lectura del ADC: ");

  ledcWriteTone(channel, 2000);
  
  /*if (potVal < 2047) { // ADC menor a 2047 reproduce melodia 1
    for ( i=0; i<MAX_COUNT1; i++) {
      toneM = melody1[i];
      beat = beats1[i];
      duration = beat * tempo; // Set up timin
      playTone(); // A pause between notes
      delayMicroseconds(pauses);
      }
  }
  else {   // Segunda melidia
    for ( i=0; i<MAX_COUNT2; i++) {
        toneM = melody2[i];
        beat = beats2[i];    
        duration = beat * tempo; // Set up timing 
        playTone(); // A pause between notes
        delayMicroseconds(pauses);
        }
     } */
   delay(200);
}
