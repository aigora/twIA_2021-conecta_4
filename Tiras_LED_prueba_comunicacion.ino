#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//Designamos nuestro pin de datos
#define PIN 6
//Designamos cuantos pixeles tenemos en nuestra cinta led RGB
#define NUMPIXELS      43

//Definimos el número de pixeles de la cinta y el pin de datos
//   Parámetro 1 = número de pixeles de la cinta
//   Parámetro 2 = número de pin de datos del Arduino
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//Definimos nuestras variables de tipo uint32 para cada color que utilizaremos
// pixels.Color toma valores en RGB, desde 0,0,0 hasta 255,255,255
uint32_t rojo = pixels.Color(5,0,0);
uint32_t azul = pixels.Color(0,0,5);
uint32_t apagado = pixels.Color(0,0,0);

void setup() {
pixels.begin();
pixels.show();
Serial.begin(9600);
}

void loop() {

if (Serial.available() > 0)
   procesar_mensaje();
}

void procesar_mensaje(void)
{
  int n1, n2;
  String jugador = Serial.readStringUntil('\n'); // Lee mensaje
  String LED_posicion = Serial.readStringUntil('\n');  // Lee valor
  
  n1 = jugador.toInt();  // Transforma valor a entero
  n2 = LED_posicion.toInt();

  if (n1 >= 0 && n1 <= 2)
     Serial.println("OK");
   
  if(n1==0)
    pixels.setPixelColor(n2, apagado);   //Apaga el LED
  
  if(n1==1)
    pixels.setPixelColor(n2, azul); // Brillo moderado en azul
    
  if(n1==2)
    pixels.setPixelColor(n2, rojo); // Brillo moderado en rojo
     
  pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB 
  
  
}
