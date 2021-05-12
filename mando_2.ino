#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <IRremote.h>
//Designamos los botones del mando
#define Boton_1 0xFF30CF
#define Boton_2 0xFF18E7
#define Boton_3 0xFF7A85
#define Boton_4 0xFF10EF
#define Boton_5 0xFF38C7
#define Boton_6 0xFF5AA5
#define Boton_7 0xFF42BD
#define Boton_0 0xFF6897
#define Boton_ARRIBA 0xFF629D
#define Boton_ABAJO 0xFFA857
#define Boton_IZQ 0xFF22DD
#define Boton_DER 0xFFC23D
#define Boton_OK 0xFF02FD
#define Boton_ON_OFF 0xFFA25D
#define Boton_FUNCTION 0xFFE21D

//Designamos nuestro pin de datos
int PIN=6;
//Designamos cuantos pixeles tenemos en nuestra cinta led RGB
int NUMPIXELS=43;

//Definimos el número de pixeles de la cinta y el pin de datos
//   Parámetro 1 = número de pixeles de la cinta
//   Parámetro 2 = número de pin de datos del Arduino
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//Definimos nuestras variables de tipo uint32 para cada color que utilizaremos
// pixels.Color toma valores en RGB, desde 0,0,0 hasta 255,255,255
uint32_t rojo = pixels.Color(5,0,0);
uint32_t azul = pixels.Color(0,0,5);
uint32_t apagado = pixels.Color(0,0,0);

//Definicion del pin del mando
int SENSOR = 11;
IRrecv irrecv(SENSOR);
decode_results codigo;



int i;
int f_1=0, f_2=1, f_3=2, f_4=3, f_5=4, f_6=5, f_7=6;



void setup() {
pixels.begin();
pixels.show();
Serial.begin(9600);
irrecv.enableIRIn();
}

void loop() {

if (irrecv.decode(&codigo)){
  Serial.println(codigo.value, HEX);
 
  if(codigo.value==Boton_1){
    pixels.setPixelColor(f_1,0,0,5);
    f_1 = f_1+7;
  }
  else if(codigo.value==Boton_2){
    pixels.setPixelColor(f_2,0,0,5);
    f_2 = f_2+7;
  }
  else if(codigo.value==Boton_3){
    pixels.setPixelColor(f_3,0,0,5);
    f_3 = f_1+3;
  }
  else if(codigo.value==Boton_4){
    pixels.setPixelColor(f_4,0,0,5);
    f_4 = f_4+7;
  }
  else if(codigo.value==Boton_5){
    pixels.setPixelColor(f_5,0,0,5);
    f_5 = f_5+7;
  }
  else if(codigo.value==Boton_6){
    pixels.setPixelColor(f_6,0,0,5);
    f_6 = f_6+7;
  }
  else if(codigo.value==Boton_7){
    pixels.setPixelColor(f_7,0,0,5);
    f_7 = f_7+7;
  }
  
  pixels.show();
  irrecv.resume();
  
  }
  delay(100);
}
