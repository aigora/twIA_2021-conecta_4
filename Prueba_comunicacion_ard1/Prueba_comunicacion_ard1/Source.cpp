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
void monitorizar_aforo(Serial*);

int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que est� conectado Arduino , cambiar en funci�n del puerto utilizado
	int d1, d2;  // Opci�n del men� principal seleccionada
	char pchar[]="HOLA";

	// Tareas de configuraci�n y carga
	
	Arduino = new Serial((char*)puerto);  // Establece la conexi�n con Arduino

	while (1)
	{
		printf("Introduzca un dato1 y un dato2: ");
		scanf_s("%d %d", &d1, &d2);

		Send_and_Receive(Arduino, d1, d2, pchar);
    }

	// Tareas de desconexi�n y cierre 
	return 0;
}

int menu_principal(void)
{
	int opcion;
	do
	{
		printf("1 - T�tulo opci�n\n");
		printf("2 - Titulo opci�n\n");
		printf("3 - Titulo opci�n\n");
		printf("4 - Titulo opci�n\n");
		printf("5 - Titulo opci�n\n");
		printf("6 - Cerrar Aplicaci�n\n");
		printf("Seleccione opci�n:");
		scanf_s("%d", &opcion);
		if (opcion < 1 || opcion>6)
			printf("\nOpci�n inexistente.\n\n");
	} while (opcion < 1 || opcion>6);
	return opcion;
}



// Ejemplo de funci�n de intercambio de datos con Arduino
/*void Talk_with_Arduino(Serial* Arduino)
{
	//char BufferSalida[MAX_BUFFER];
	char BufferEntrada[MAX_BUFFER];
	int bytesReceive, numero_recibido;

	if (Arduino->IsConnected()) // Si hay conexi�n con Arduino 
	{

		// Para enviar un mensaje y obtener una respuesta se utiliza la funci�n Send_and_Receive
		// El mensaje est� formado por un texto y un entero
		// El mensaje que se recibe est� formado tambi�n por un texto y un entero.
		// Par�metros de la funci�n:
		// El primero es la referencia a Arduino
		// El segundo es el mensaje que se desea enviar
		// El tercero es un entero que complementa al mensaje que se desea enviar
		// El cuarto es el vector de char donde se recibe la respuesta
		// El quinto es la referencia donde se recibe el entero de la respuesta
		// La funci�n devuelve un entero con los bytes recibidos. Si es cero no se ha recibido nada.

		bytesReceive = Send_and_Receive(Arduino, "GET_AFORO_MAX", -1, BufferEntrada, &numero_recibido);
		if (bytesReceive == 0)
			printf("No se ha recibido respuesta al mensaje enviado\n");
		else
			printf("Mensaje recibido %s %d\n", BufferEntrada, numero_recibido);
	}
	else
		printf("La comunicaci�n con la plataforma hardware no es posible en este momento\n"); // Req 3
}
*/
// Protocolo de intercambio mensajes entre Pc y platforma hardware
// Env�o Mensaje valor
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


// Env�a un mensaje a la plataforma hardware
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
