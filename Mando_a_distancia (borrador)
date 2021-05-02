#include <IRremote.h>
#define Boton_1 0xFF30CF
#define Boton_2 0xFF18E7
#define Boton_3 0xFF7A85

int SENSOR = 11;
IRrecv irrecv(SENSOR);
decode_results codigo;
int LEDR = 4;
int LEDV = 3; 
int LEDA = 2; 

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode (LEDR, OUTPUT); 
  pinMode (LEDV, OUTPUT);
  pinMode (LEDA, OUTPUT);
}

void loop(){
  if (irrecv.decode(&codigo)){
    Serial.println(codigo.value, HEX);
    
    if (codigo.value == Boton_1)
    digitalWrite(LEDR, !digitalRead(LEDR));

    if (codigo.value == Boton_2)
    digitalWrite(LEDV, !digitalRead(LEDV));
    
    if (codigo.value == Boton_3)
    digitalWrite(LEDA, !digitalRead (LEDA));

    
    irrecv.resume();
  }
  delay(100);
}
