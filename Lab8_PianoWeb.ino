#include <ESPAsyncWebServer.h>

// Creamos el servidor AsyncWebServer en el puerto 80
AsyncWebServer server(80);

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

void setup() { 
    pinMode(speakerOut, OUTPUT);
    Serial.begin(115200); // Set serial out if we want debugging
  } 

void loop() {
  potVal = analogRead(potPin); // Lectura del dato del ADC
  Serial.println(potVal);      // Print potVal en serial monitor
  
  if (potVal < 2047) { // ADC menor a 2047 reproduce melodia 1
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
     } 
   delay(200);
}
