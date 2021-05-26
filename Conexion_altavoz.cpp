#include <stdio.h>
#include <stdlib.h>
#include "SerialClass/SerialClass.h"

int main()
{
    Serial* Arduino; 

    char puerto[]="COM3"; // depende del puerto
    int cancion = 0;
    char secuencia[2];

    Arduino = new Serial((char *)puerto);
    // Colocar esta seccion en el lugar donde se quierre que se escuche la canción 
    if (Arduino->IsConnected())
    {

        cancion = 1; //INICIO

        sprintf_s(secuencia, "%d", cancion);

        Arduino->WriteData(secuencia, strlen(secuencia));
        Sleep(2000);
    }

    // Colocar esta seccion en el lugar donde se quierre que se escuche la canción 
    if (Arduino->IsConnected())
    {

        cancion = 2; //VICTORIA

        sprintf_s(secuencia, "%d", cancion);

        Arduino->WriteData(secuencia, strlen(secuencia));
        Sleep(2000);

    }
    
}

