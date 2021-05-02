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

void setup() {
pixels.begin();
  
 }

void loop() {

//Definimos nuestras variables de tipo uint32 para cada color que utilizaremos
// pixels.Color toma valores en RGB, desde 0,0,0 hasta 255,255,255
uint32_t rojo = pixels.Color(5,0,0);
uint32_t verde = pixels.Color(0,5,0);
uint32_t azul = pixels.Color(0,0,5);

int i, j, led;
FILE *mifichero;

mifichero = fopen("   ","r");

if(mifichero==NULL)
   printf("Se ha producido un error");
else
{
    for (i=0;i<2;i++)
    {
      fscanf(mifichero,"%d",&j);
      fscanf(mifichero,"%d",&led);

      if(j==1)
      { pixels.setPixelColor(led, azul); // Brillo moderado en rojo
        pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
      }
      if(j==2)
      {
        pixels.setPixelColor(led, rojo); // Brillo moderado en rojo
        pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB 
      }
        
    }
    
}

}
