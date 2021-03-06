#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include "SerialClass.h"

#define MAX_BUFFER 200
#define MAX_INTENTOS_READ 4
#define MS_ENTRE_INTENTOS 250
#define SI 1
#define NO 0

// Funciones prototipo
int menu_principal(void);


//void Talk_with_Arduino(Serial* Arduino);
void Send_to_hw(Serial*, char*);
int Receive_from_hw(Serial* Arduino, char* BufferEntrada);
int Send_and_Receive(Serial* Arduino, int msg_out, int valor_out, char* msg_in);
int Recibir_mando_ard(Serial* Arduino, char* msg_in, int* valor_in);
int leer_boton(int);


int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que está conectado Arduino , cambiar en función del puerto utilizado
	char pchar[MAX_BUFFER];
	int x,columna;

	// Tareas de configuración y carga
	
	Arduino = new Serial((char*)puerto);  // Establece la conexión con Arduino

	while (1)
	{
		
         if (Recibir_mando_ard(Arduino, pchar, &x) != 0)
	 {
			printf("%s %d\n", pchar, x);
			
			columna = leer_boton(x);

			if (columna >= 1 && columna <= 7)
				printf("\nColumna leida es %d\n\n", columna);
	 }
    }
	
	

	// Tareas de desconexión y cierre 
	return 0;
}

int menu_principal(void)
{
	int opcion;
	do
	{
		printf("1 - Título opción\n");
		printf("2 - Titulo opción\n");
		printf("3 - Titulo opción\n");
		printf("4 - Titulo opción\n");
		printf("5 - Titulo opción\n");
		printf("6 - Cerrar Aplicación\n");
		printf("Seleccione opción:");
		scanf_s("%d", &opcion);
		if (opcion < 1 || opcion>6)
			printf("\nOpción inexistente.\n\n");
	} while (opcion < 1 || opcion>6);
	return opcion;
}

int leer_boton(int x)
{
	int c;

	switch (x)
	{
	case 12:
		c = 1;
		break;
	case 24:
		c = 2;
		break;
	case 94:
		c = 3;
		break;
	case 8:
		c = 4;
		break;
	case 28:
		c = 5;
		break;
	case 90:
		c = 6;
		break;
	case 66:
		c = 7;
		break;
	default:
		c = -1;
	}

    return c;

}


// Ejemplo de función de intercambio de datos con Arduino
/*void Talk_with_Arduino(Serial* Arduino)
{
	//char BufferSalida[MAX_BUFFER];
	char BufferEntrada[MAX_BUFFER];
	int bytesReceive, numero_recibido;

	if (Arduino->IsConnected()) // Si hay conexión con Arduino 
	{

		// Para enviar un mensaje y obtener una respuesta se utiliza la función Send_and_Receive
		// El mensaje está formado por un texto y un entero
		// El mensaje que se recibe está formado también por un texto y un entero.
		// Parámetros de la función:
		// El primero es la referencia a Arduino
		// El segundo es el mensaje que se desea enviar
		// El tercero es un entero que complementa al mensaje que se desea enviar
		// El cuarto es el vector de char donde se recibe la respuesta
		// El quinto es la referencia donde se recibe el entero de la respuesta
		// La función devuelve un entero con los bytes recibidos. Si es cero no se ha recibido nada.

		bytesReceive = Send_and_Receive(Arduino, "GET_AFORO_MAX", -1, BufferEntrada, &numero_recibido);
		if (bytesReceive == 0)
			printf("No se ha recibido respuesta al mensaje enviado\n");
		else
			printf("Mensaje recibido %s %d\n", BufferEntrada, numero_recibido);
	}
	else
		printf("La comunicación con la plataforma hardware no es posible en este momento\n"); // Req 3
}
*/
// Protocolo de intercambio mensajes entre Pc y platforma hardware
// Envío Mensaje valor
// Recibe Mensaje valor
// Retorna bytes de la respuesta (0 si no hay respuesta)
int Send_and_Receive(Serial* Arduino, int msg_out, int valor_out, char* msg_in)
{
	char BufferSalida[MAX_BUFFER];
	char BufferEntrada[MAX_BUFFER];

	int bytesReceive;

	sprintf_s(BufferSalida, "%d\n%d\n", msg_out, valor_out);
	Send_to_hw(Arduino, BufferSalida);
	bytesReceive = Receive_from_hw(Arduino, BufferEntrada);
	if (bytesReceive != 0)
		strcpy_s(msg_in, MAX_BUFFER, BufferEntrada);

	return bytesReceive;
}


// Envía un mensaje a la plataforma hardware
void Send_to_hw(Serial* Arduino, char* BufferSalida)
{
	Arduino->WriteData(BufferSalida, strlen(BufferSalida));
}

//Recibe (si existe) un mensaje de la plataforma hardware
//Realiza MAX_INTENTOS_READ para evitar mensajes recortados
int Receive_from_hw(Serial* Arduino, char* BufferEntrada)
{
	int bytesRecibidos, bytesTotales = 0;
	int intentos_lectura = 0;
	char cadena[MAX_BUFFER];

	BufferEntrada[0] = '\0';
	while (intentos_lectura < MAX_INTENTOS_READ)
	{
		cadena[0] = '\0';
		bytesRecibidos = Arduino->ReadData(cadena, sizeof(char) * (MAX_BUFFER - 1));
		if (bytesRecibidos != -1)
		{
			cadena[bytesRecibidos] = '\0';
			strcat_s(BufferEntrada, MAX_BUFFER, cadena);
			bytesTotales += bytesRecibidos;
		}
		intentos_lectura++;
		Sleep(MS_ENTRE_INTENTOS);
	}
	return bytesTotales;
}

int Recibir_mando_ard(Serial* Arduino, char* msg_in, int* valor_in)
{
	char BufferSalida[MAX_BUFFER];
	char BufferEntrada[MAX_BUFFER];
	char* ptr;
	int bytesReceive;

	
	
	bytesReceive = Receive_from_hw(Arduino, BufferEntrada);
	if (bytesReceive != 0)
	{
		ptr = strpbrk(BufferEntrada, " ");
		if (ptr == NULL)
			*valor_in = -1;
		else
		{
			*valor_in = atoi(ptr);
			*ptr = '\0';
		}
		strcpy_s(msg_in, MAX_BUFFER, BufferEntrada);
	}
	return bytesReceive;
}
