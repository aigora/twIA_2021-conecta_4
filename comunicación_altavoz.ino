int altavoz=7;
int i;
char opcion;

void setup () 
{
     Serial.begin(9600);
     pinMode (altavoz,OUTPUT);
}
void loop ()
{
     if( Serial.available()> 0) 
     {
        opcion = Serial.read();
   
        if(opcion == '1'){            //sonido START
            tone (altavoz,400,300); // primero el tono y luego la duracion 
            delay(250);
            tone (altavoz,450,500); 

        }
        if(opcion == '2'){
           for (int i=1; i<10; i++){   //sonido victoria
            tone (altavoz,i*50,100);
            delay(200); 
            }
            i = 0;
            for (int i=1; i<10; i++){   
            tone (altavoz,i*50,100); 
            delay(30); 
            }  

        }


     }
}
