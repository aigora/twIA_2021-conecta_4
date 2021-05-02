  int a = 0;
  int altavoz = 0;   //aqui va el pin al que esta conectado el altavoz

void setup() { 
  pinMode (0,OUTPUT);  //aqui va el pin al que esta conectado el altavoz


}

void loop() {  
  
  if (a == 1){  
    tone (altavoz,440,500); // primero el tono y luego la duracion 
    a = 0;
  }
  if (a == 2){ 
    a = 0; 
    for (int i=1; i<10; i++){   //sonido victoria
    tone (altavoz,i*50,100);
    delay(200);  
  } 
  }   
}
