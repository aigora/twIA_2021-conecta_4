#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <IRremote.h>
#include <math.h>
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



int i=1;
int f_1=0, f_2=1, f_3=2, f_4=3, f_5=4, f_6=5, f_7=6;



void setup() {
pixels.begin();
pixels.show();
Serial.begin(9600);
while(!Serial) { ; }  //Comprueba el Serial mientras sea falso, que significa que no está conectado
irrecv.enableIRIn();
}

void loop() {

if (Serial.available() > 0)
   procesar_mensaje();

if (irrecv.decode(&codigo))
{
  Serial.println("Boton_pulsado ");  //Mensaje enviado
  Serial.println(codigo.value,HEX);  //Código del mando en hexadecimal
 
  jugar1();
 
  
  pixels.show();
  irrecv.resume();
  
}
  delay(100);
}

void jugar1 (void){
  if(i%2==0){
    if(codigo.value==Boton_1){
    pixels.setPixelColor(f_1,azul);
    f_1 = f_1+7;
    i++;
  }
   if(codigo.value==Boton_2){
    pixels.setPixelColor(f_2,azul);
    f_2 = f_2+7;
    i++;
  }
   if(codigo.value==Boton_3){
    pixels.setPixelColor(f_3,azul);
    f_3 = f_3+7;
    i++;
  }
   if(codigo.value==Boton_4){
    pixels.setPixelColor(f_4,azul);
    f_4 = f_4+7;
    i++;
  }
   if(codigo.value==Boton_5){
    pixels.setPixelColor(f_5,azul);
    f_5 = f_5+7;
    i++;
  }
   if(codigo.value==Boton_6){
    pixels.setPixelColor(f_6,azul);
    f_6 = f_6+7;
    i++;
  }
   if(codigo.value==Boton_7){
    pixels.setPixelColor(f_7,azul);
    f_7 = f_7+7;
    i++;
  }
}
 else{
    if(codigo.value==Boton_1){
    pixels.setPixelColor(f_1,rojo);
    f_1 = f_1+7;
    i++;
  }
  if(codigo.value==Boton_2){
    pixels.setPixelColor(f_2,rojo);
    f_2 = f_2+7;
    i++;
  }
   if(codigo.value==Boton_3){
    pixels.setPixelColor(f_3,rojo);
    f_3 = f_3+7;
    i++;
  }
   if(codigo.value==Boton_4){
    pixels.setPixelColor(f_4,rojo);
    f_4 = f_4+7;
    i++;
  }
   if(codigo.value==Boton_5){
    pixels.setPixelColor(f_5,rojo);
    f_5 = f_5+7;
    i++;
  }
   if(codigo.value==Boton_6){
    pixels.setPixelColor(f_6,rojo);
    f_6 = f_6+7;
    i++;
  }
   if(codigo.value==Boton_7){
    pixels.setPixelColor(f_7,rojo);
    f_7 = f_7+7;
    i++;
  }
  }
  }

  void procesar_mensaje(void)
{
  int n1, n2;
  String jugador = Serial.readStringUntil('\n'); // Lee mensaje
  String LED_posicion = Serial.readStringUntil('\n');  // Lee valor
  
  n1 = jugador.toInt();  // Transforma valor a entero
  n2 = LED_posicion.toInt();  // Transforma valor a entero

  if(n1==0)
    //pixels.setPixelColor(n2, apagado);   //Apaga el LED
    
    pixels.clear();
  
  if(n1==1)
    pixels.setPixelColor(n2, azul); // Brillo moderado en azul
    
  if(n1==2)
    pixels.setPixelColor(n2, rojo); // Brillo moderado en rojo
     
  pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB 
  
  
}
