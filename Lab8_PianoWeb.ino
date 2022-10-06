#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "config.h"

// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Star Wars TONES //
// Definiendo la relacion entre note, period & frequency. 
// periodo en un microsegundo P = 1/f * (1E6)
#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517    // 659 Hz
#define  f5    1433    // 698 Hz
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
 
#define  R     0      // Definiendo la nota especial, 'R', to represent a rest
 
int speakerOut = 18;    
int potPin = A0;      // entrada analogica
 
// Melody 1: Star Wars Marcha imperial
int melody1[] = {  a4, R,  a4, R,  a4, R,  f4, R, c5, R,  a4, R,  f4, R, c5, R, a4, R,  e5, R,  e5, R,  e5, R,  f5, R, c5, R,  g5, R,  f5, R,  c5, R, a4,  R, R  };
int beats1[]  = {  50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5,  20, 5, 60, 40, 300};
// Melody 2: Star Wars
int melody2[] = {  f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s,  c5, R};
int beats2[]  = {  21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128, 300 }; 
 
int MAX_COUNT1 = 37; // 
int MAX_COUNT2 = 20; //
long tempo = 11000;  //
int pauses = 1000; //   distancia entres notas
int rest_count = 50; //
// Inicializando variables
int toneM = 0,beat = 0,potVal = 0,i = 0;
long duration  = 0;

// PLAY TONE  //
void playTone() {
  long elapsed_time = 0;
  if (toneM > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(toneM / 2);
      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(toneM / 2); 
      // Keep track of how long we pulsed
      elapsed_time += (toneM);
    } 
  }                         
}

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
  pinMode(speakerOut, OUTPUT);
  Serial.begin(115200); // Set serial out if we want debugging

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
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 2
  server.on("/TECLA2", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 2 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 3
  server.on("/TECLA3", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 3 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 4
  server.on("/TECLA4", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 4 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 5
  server.on("/TECLA5", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 5 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 6
  server.on("/TECLA6", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 6 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 7
  server.on("/TECLA7", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 7 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 8
  server.on("/TECLA8", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 8 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 9
  server.on("/TECLA9", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 9 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 10
  server.on("/TECLA10", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 10 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 11
  server.on("/TECLA11", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 11 presionada");
    //request->send_P(200, "text/plain",getRSSI().c_str());
  });
  // Tecla 12
  server.on("/TECLA12", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Tecla 12 presionada");
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
  potVal = analogRead(potPin); // Lectura del dato del ADC
  Serial.print("Lectura del ADC: ");
  Serial.println(potVal);      // Print potVal en serial monitor
  
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
