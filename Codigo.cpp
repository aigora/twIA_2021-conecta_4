#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <locale.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include "SerialClass.h"

#define MAX_BUFFER 200
#define MAX_INTENTOS_READ 4
#define MS_ENTRE_INTENTOS 250
#define SI 1
#define NO 0
#define N 10
#define FILAS 6
#define COLUMN 7
#define LONG_CAD 50

typedef struct
{
	char nombre[LONG_CAD];
	char apellidos[LONG_CAD];
	char username[LONG_CAD];
	char password[LONG_CAD];

} Usuario;

// Función prototipo
int menu_principal(void);

// Funciones relacionadas con la partida
void inicializar_tablero(int[][COLUMN]);
void imprimir_tablero(int[][COLUMN]);
void un_jugador(int[][COLUMN], Usuario*);
void dos_jugadores(int[][COLUMN], Usuario*);
void meter_ficha(int[][COLUMN], int, int*, Usuario*);
void IA(int[][COLUMN], int, int*);
int conecta(int, int, int, int[][COLUMN]);
int contar_vertical(int, int, int, int[][COLUMN]);
int contar_horizontal(int, int, int, int[][COLUMN]);
int contar_diagonal_ascendente(int, int, int, int[][COLUMN]);
int contar_diagonal_descendente(int, int, int, int[][COLUMN]);
void guarda_partida(int[][COLUMN]);
void reaunudar_partida(int[][COLUMN]);
void Send_to_hw(Serial*, char*);
int Receive_from_hw(Serial* Arduino, char* BufferEntrada);
int Send_and_Receive(Serial* Arduino, int msg_out, int valor_out, char* msg_in);
int Recibir_mando_ard(Serial* Arduino, char* msg_in, int* valor_in);
int leer_boton(int);

// Funciones de gestión de usuarios
Usuario* leer_fichero_usuarios(int*);
int escribir_fichero_usuarios(Usuario*, int*);
Usuario* gestion_usuarios(Usuario*, int*);
void listado_usuarios(Usuario*, int);
Usuario* alta_usuario(Usuario*, int*);
int baja_usuario(Usuario*, int*);
int consulta_usuario(Usuario*, int, char*);
int consulta_password(Usuario*, int, char*);
int posicion_usuario(Usuario*, int, char*);
int posicion_password(Usuario*, int, char*);
int puntuaciones_jugador1(Usuario*, int, int[][COLUMN], int*);
int puntuaciones_jugador2(Usuario*, int, int[][COLUMN], int*);

int main(void)
{
	// comunicacion arduino
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que está conectado Arduino , cambiar en función del puerto utilizado
	char pchar[MAX_BUFFER];
	Arduino = new Serial((char*)puerto);

	int opc; // Opción del menú principal
	int num_usuarios; // Cantidad de usuarios actual
	Usuario* usuarios; // Lista de usuarios actual
	int tablero[FILAS][COLUMN];
	int* fin = 0;
	int i;
	int username, password;
	char intro;
	char usuario1[LONG_CAD], contrasena1[LONG_CAD], usuario2[LONG_CAD], contrasena2[LONG_CAD];

	// Leer fichero con los usuarios
	usuarios = leer_fichero_usuarios(&num_usuarios); // Traslada los usuarios desde un fichero a memoria

	// Bucle principal de la aplicación
	do
	{
		opc = menu_principal();
		switch (opc)
		{
		case 1:
			usuarios = gestion_usuarios(usuarios, &num_usuarios);
			break;
		case 2:
			do
			{
				printf("==========\n");
				printf("Username: ");
				intro = getchar();
				gets_s(usuario1, LONG_CAD);
				printf("Password: ");
				gets_s(contrasena1, LONG_CAD);

				username = consulta_usuario(usuarios, num_usuarios, usuario1);
				password = consulta_password(usuarios, num_usuarios, contrasena1);

			} while (username != 0 || password != 0);
			un_jugador(tablero, usuarios);
			break;
		case 3:
			do
			{
				printf("==========\n");
				printf("Username 1: ");
				intro = getchar();
				gets_s(usuario1, LONG_CAD);
				printf("Password 1: ");
				gets_s(contrasena1, LONG_CAD);

				username = consulta_usuario(usuarios, num_usuarios, usuario1);
				password = consulta_password(usuarios, num_usuarios, contrasena1);

			} while (username != 0 || password != 0);
			do
			{
				printf("==========\n");
				printf("Username 2: ");
				intro = getchar();
				gets_s(usuario2, LONG_CAD);
				printf("Password 2: ");
				gets_s(contrasena2, LONG_CAD);

				username = consulta_usuario(usuarios, num_usuarios, usuario2);
				password = consulta_password(usuarios, num_usuarios, contrasena2);

			} while (username != 0 || password != 0);

			dos_jugadores(tablero, usuarios);
			break;
		case 4:
			reaunudar_partida(tablero);
			break;
		case 5:
			if (opc == 2)
			{
				puntuaciones_jugador1(usuarios, num_usuarios, tablero, fin);
			}
			else
			{
				if (opc == 3)
				{
					puntuaciones_jugador1(usuarios, num_usuarios, tablero, fin);
					puntuaciones_jugador2(usuarios, num_usuarios, tablero, fin);
				}
			}
			break;
		case 6:
			break;
		}
	} while (opc != 5);

	// Tareas de desconexión y cierre
	escribir_fichero_usuarios(usuarios, &num_usuarios); // Traslada los usuarios desde memoria a un fichero
}

int menu_principal(void)
{
	int opcion;

	do
	{
		printf("Menu principal Conecta-4\n");
		printf("========================\n");
		printf("1 - Gestion de usuarios\n");
		printf("2 - 1 Jugador\n");
		printf("3 - 2 Jugadores\n");
		printf("4 - Reaunudar partida\n");
		printf("5 - Ver puntuaciones\n");
		printf("6 - Salir\n");
		printf("Seleccione una opcion: ");
		scanf_s("%d", &opcion);

		if (opcion < 1 || opcion>6)
			printf("\nOpcion inexistente.\n\n");
	} while (opcion < 1 || opcion>6);

	return opcion;
}

void inicializar_tablero(int tablero[][COLUMN]) {
	int i, j;
	for (i = 0; i < 6; i++) // en este bucle ponermos el tablero a 0 (casillas vacías)
	{
		for (j = 0; j < 7; j++)
		{
			tablero[i][j] = 0;
		}
	}
}

void imprimir_tablero(int tablero[][COLUMN]) {
	int i, j;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			printf("|%d ", tablero[i][j]);
		}
		printf("|\n");
	}
}

void un_jugador(int tablero[][COLUMN], Usuario* lista) {
	int turno = 1;
	int fin = 0;
	int cancion = 0;
	char secuencia[2];
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que está conectado Arduino , cambiar en función del puerto utilizado
	char pchar[MAX_BUFFER];
	Arduino = new Serial((char*)puerto);

	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	printf("\nHa seleccionado modo 1 jugador\n");
	printf("Inicio de la partida:\n");
	if (Arduino->IsConnected())
	{

		cancion = 1;

		sprintf_s(secuencia, "%d", cancion);

		Arduino->WriteData(secuencia, strlen(secuencia));
		Sleep(2000);
	}
	inicializar_tablero(tablero);//Inicializamos tablero
	printf("\n");
	do {
		printf("\nTURNO %d", turno); //Indica cuantos turnos llevan
		printf("\n");
		printf("Inicio del turno del jugador\n");
		meter_ficha(tablero, 1, &fin, lista); //El jugador mete ficha
		imprimir_tablero(tablero);
		if (fin == 0) { //El jugador 2 solo puede meter ficha si el jugador 1 no ha ganado
			printf("Inicio del turno de la CPU\n");
			IA(tablero, 2, &fin);
			imprimir_tablero(tablero);
			turno++;
		}
	} while (fin == 0);
}

void dos_jugadores(int tablero[][COLUMN], Usuario* lista) {
	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	int turno = 1;
	int fin = 0;
	int cancion = 0;
	char secuencia[2];
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que está conectado Arduino , cambiar en función del puerto utilizado
	char pchar[MAX_BUFFER];
	Arduino = new Serial((char*)puerto);

	printf("\nHa seleccionado modo 2 jugadores\n");
	printf("Inicio de la partida:\n");
	if (Arduino->IsConnected())
	{

		cancion = 1;

		sprintf_s(secuencia, "%d", cancion);

		Arduino->WriteData(secuencia, strlen(secuencia));
		Sleep(2000);
	}
	inicializar_tablero(tablero);//Inicializamos tablero
	printf("\n");
	do  // el bucle de turnos continúa hasta que se detecte alguna jugada como victoria para un jugador
	{
		printf("\nTURNO %d", turno); //Indica cuantos turnos llevan
		printf("\n");
		printf("Inicio del turno del jugador 1\n");
		meter_ficha(tablero, 1, &fin, lista);//El jugador elige dónde poner la ficha
		imprimir_tablero(tablero);
		if (fin == 0) // la ficha del seundo jugador solo se puede introducir si no ha ganado el jugador 1
		{
			printf("Inicio del turno del jugador 2\n");
			meter_ficha(tablero, 2, &fin, lista);//El jugador elige dónde poner la ficha
			imprimir_tablero(tablero);
			turno++;
		}
	} while (fin == 0);
	if (Arduino->IsConnected())
	{

		cancion = 2;

		sprintf_s(secuencia, "%d", cancion);

		Arduino->WriteData(secuencia, strlen(secuencia));
		Sleep(2000);

	}
}

void meter_ficha(int tablero[][COLUMN], int jugador, int* fin, Usuario* lista)//Pone la ficha del jugador "jugador" em el tablero
{
	int fila, columna = 0, exito = 0, x; // declaramos las variables
	char letra[2], intro;
	Serial* Arduino;
	char puerto[] = "COM5"; //Puerto serie al que está conectado Arduino , cambiar en función del puerto utilizado
	char pchar[MAX_BUFFER];
	Arduino = new Serial((char*)puerto);
	printf("Desea guardar la partida (S/N): ");
	gets_s(letra, 2);

	if (strcmp(letra, "S") == 0 || strcmp(letra, "s") == 0)
	{
		guarda_partida(tablero);

		exit(0);
	}
	else
	{
		do //repetimos la funcion hasta que el usuario introduzca una columna válida
		{
			printf("Introduzca columna:\n");
			while (columna == 0)
			{

				if (Recibir_mando_ard(Arduino, pchar, &x) != 0)
				{
					printf("%s %d\n", pchar, x);
					columna = leer_boton(x);
				}
			}
			intro = getchar();
			columna--; // ajustamos columna a un valor inferior para que las columnas sean del 1 al 7, más intuitivo para el usuario
			if (columna < 0 || columna >(COLUMN - 1))
				printf("Columna no valida-limites 1 a %d-\n", COLUMN - 1);
			else
			{
				if (tablero[0][columna] != 0) {//Si la columna esta completa
					printf("Columna no valida-esta completa-\n");
					columna = -1;//Truco para forzar el nuevo bucle
				}
			}
		} while (columna < 0 || columna >(COLUMN - 1));
		for (fila = FILAS - 1; fila >= 0 && exito == 0; fila--) {//Buscamos fila con un hueco en la columna
			if (tablero[fila][columna] == 0) {//Si está vacía
				tablero[fila][columna] = jugador;//Ponemos ficha del jugador
				exito = 1;
				if (conecta(fila, columna, jugador, tablero) != 0) { //Comprueba si el jugador puede ganar
					printf("Ha ganado el jugador %d\n", jugador); //Indica quien es el jugador que ha ganado
					*fin = jugador;
				}
			}
		}

	}

	puntuaciones_jugador1(lista, jugador, tablero, fin);
	puntuaciones_jugador2(lista, jugador, tablero, fin);

}

void IA(int tablero[][COLUMN], int jugador, int* fin)
{
	int fila, columna, exito = 0;
	do
	{
		if (exito == 0)
		{
			for (columna = 0; columna < COLUMN; columna++)
			{
				if (tablero[FILAS - 4][columna] == 0 && tablero[FILAS - 3][columna] == jugador && tablero[FILAS - 2][columna] == jugador && tablero[FILAS - 1][columna] == jugador)// en caso de que hayamos ocupado las tres primeras filas, colocar una ficha encima de esta
				{
					tablero[FILAS - 4][columna] = jugador;//Ponemos ficha del jugador
					exito = 1; // salimos del bucle while
					if (conecta(FILAS - 4, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
					columna = COLUMN; // truco para cerrar el bucle for
				}
			}
		}
		if (exito == 0)
		{
			for (columna = 0; columna < COLUMN; columna++)
			{
				if (tablero[FILAS - 3][columna] == 1 && tablero[FILAS - 2][columna] == 1 && tablero[FILAS - 1][columna] == 1)// en caso de que hayamos ocupado las dos primeras filas, colocar una ficha encima de esta
				{
					tablero[FILAS - 4][columna] = jugador;//Ponemos ficha del jugador
					exito = 1; // salimos del bucle while
					if (conecta(FILAS - 4, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
					columna = COLUMN; // truco para cerrar el bucle for
				}
			}
		}
		if (exito == 0)
		{
			for (columna = 0; columna < COLUMN; columna++)
			{
				if (tablero[FILAS - 3][columna] == 0 && tablero[FILAS - 2][columna] == jugador && tablero[FILAS - 1][columna] == jugador)// en caso de que hayamos ocupado las dos primeras filas, colocar una ficha encima de esta
				{
					tablero[FILAS - 3][columna] = jugador;//Ponemos ficha del jugador
					exito = 1; // salimos del bucle while
					if (conecta(FILAS - 3, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
					columna = COLUMN; // truco para cerrar el bucle for
				}
			}
		}
		if (exito == 0)
		{
			for (columna = 0; columna < COLUMN; columna++)
			{
				if (tablero[FILAS - 2][columna] == 0 && tablero[FILAS - 1][columna] == jugador)// en caso de que hayamos ocupado la primera fila, colocar una ficha encima de esta
				{
					tablero[FILAS - 2][columna] = jugador;//Ponemos ficha del jugador
					exito = 1;// salimos del bucle while
					if (conecta(FILAS - 2, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
					columna = COLUMN;// truco para cerrar el bucle for
				}
			}
		}
		if (exito == 0)
		{
			for (columna = 0; columna < COLUMN; columna++)
			{
				if (tablero[FILAS - 1][columna] == 0)
				{
					tablero[FILAS - 1][columna] = jugador;//Ponemos ficha del jugador
					exito = 1;// salimos del bucle while
					if (conecta(FILAS - 1, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
					columna = COLUMN;// truco para cerrar el bucle for
				}
			}
		}
		if (exito == 0)
		{
			columna = rand() % COLUMN; //Hacemos que la IA elija una columna aleatoria
			for (fila = FILAS - 1; fila >= 0 && exito == 0; fila--)//Buscamos fila con un hueco en la columna
			{
				if (tablero[fila][columna] == 0) {//Si está vacía
					tablero[fila][columna] = jugador;//Ponemos ficha del jugador
					exito = 1;// salimos del bucle while
					if (conecta(fila, columna, jugador, tablero) != 0) {
						printf("Ha ganado el jugador %d\n", jugador);
						*fin = jugador;
					}
				}
			}
		}
	} while (exito == 0);
}

int conecta(int fila, int columna, int jugador, int tablero[][COLUMN]) { //Comprueba si hay cuatro en raya
	if (contar_vertical(fila, columna, jugador, tablero) >= 4) //Si se cumple, hay cuatro fichas conectadas en vertical
		return 1;
	if (contar_horizontal(fila, columna, jugador, tablero) >= 4) //Si se cumple, hay cuatro fichas conectadas en horizontal
		return 2;
	if (contar_diagonal_ascendente(fila, columna, jugador, tablero) >= 4) //Si se cumple, hay cuatro fichas conectadas en diagonal ascendente
		return 3;
	if (contar_diagonal_descendente(fila, columna, jugador, tablero) >= 4) //Si se cumple, hay cuatro fichas conectadas en diagonal descendente
		return 4;
	else
		return 0;
}

int contar_vertical(int fila, int columna, int jugador, int tablero[][COLUMN])
{
	int contador = 0; // Contador de fichas
	int f, c; // Variables auxiliares para filas y columnas

	f = fila;
	c = columna;
	//Bucle para contar desde la ultima ficha metida hacia abajo
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		f++;
	} while (f < FILAS && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	return contador; 
}

int contar_horizontal(int fila, int columna, int jugador, int tablero[][COLUMN])
{
	int contador = 0; // Contador de fichas
	int f, c; // Variables auxiliares para filas y columnas

	f = fila;
	c = columna;
	//Bucle para contar desde la ultima ficha metida hacia la izquierda
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		c--; // Decrementamos la columna
	} while (c >= 0 && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	//Bucle para contar desde la ultima ficha metida hacia la derecha
	f = fila;
	c = columna;
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		c++;
	} while (c < COLUMN && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	return contador - 1; // Hay que restar uno al contador porque has contado dos veces la ficha
}

int contar_diagonal_ascendente(int fila, int columna, int jugador, int tablero[][COLUMN])
{
	int contador = 0; // Contador de fichas
	int f, c; // Variables auxiliares para filas y columnas
	// La diagonal ascendente tiene esta forma: '/'

	f = fila;
	c = columna;
	//Bucle para contar desde la ultima ficha metida hacia la izquierda y abajo
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		f++;
		c--;
	} while (f < FILAS && c >= 0 && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	//Bucle para contar desde la ultima ficha metida hacia la derecha y arriba
	f = fila;
	c = columna;
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		f--;
		c++;
	} while (f >= 0 && c < COLUMN && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	return contador - 1; // Hay que restar uno al contador porque has contado dos veces la ficha
}

int contar_diagonal_descendente(int fila, int columna, int jugador, int tablero[][COLUMN]) {
	int contador = 0; // Contador de fichas
	int f, c; // Variables auxiliares para filas y columnas
	// La diagonal descendiente tiene esta forma: '\'

	f = fila;
	c = columna;
	//Bucle para contar desde la ultima ficha metida hacia la izquierda y arriba
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		f--;
		c--;
	} while (f >= 0 && c >= 0 && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	//Bucle para contar desde la ultima ficha metida hacia la derecha y abajo
	f = fila;
	c = columna;
	do
	{
		if (tablero[f][c] == jugador)
			contador++;
		f++;
		c++;
	} while (f < FILAS && c < COLUMN && tablero[f][c] == jugador); // Verifica las filas y columnas antes de mirar tablero (cortocircuito)

	return contador - 1; // Hay que restar uno al contador porque has contado dos veces la ficha
}

// Menú con las opciones para gestionar usuarios
Usuario* gestion_usuarios(Usuario* lista, int* num)
{
	int opcion;

	do
	{
		printf("Gestion de Usuarios\n");
		printf("===================\n");
		printf("1 - Alta de usuario\n");
		printf("2 - Baja de usuario\n");
		printf("3 - Listado de los usuarios\n");
		printf("4 - Volver al menu principal\n");
		printf("Seleccione una opcion: ");
		scanf_s("%d", &opcion);

		switch (opcion)
		{
		case 1:
			lista = alta_usuario(lista, num);
			break;
		case 2:
			baja_usuario(lista, num);
			break;
		case 3:
			listado_usuarios(lista, *num);
			break;
		case 4:
			break;
		default: printf("Opcion inexistente");
		}
	} while (opcion != 4);

	return lista;
}

// Muestra la lista de usuarios
void listado_usuarios(Usuario* lista, int numero)
{
	int i;

	if (numero == 0)
		printf("No hay usuarios actualmente\n");
	else
	{
		printf("En este momento existen %d usuarios\n", numero);
		printf("Nombre\t\tApellidos\t\tUsername\t\tPassword\n");
		printf("======\t\t=========\t\t========\t\t========\n");

		for (i = 0; i < numero; i++)
			printf("%s\t\t%s\t\t\t%s\t\t\t%s\n", (lista + i)->nombre, (lista + i)->apellidos, (lista + i)->username, (lista + i)->password);
	}

	printf("\n\n");
}

// Consulta el username del usuario pedido
int consulta_usuario(Usuario* lista, int num, char* username)
{
	int posicion = -1;
	int cod_error = 0;
	int i;

	posicion = posicion_usuario(lista, num, username);
	if (posicion == -1)
	{
		for (i = 0; i < num && posicion == -1; i++)
		{
			if (strcmp(username, lista[i].username) == 0)
			{
				printf("No existe ningun usuario con ese username\n");
				cod_error = -1;
			}
		}
	}

	return cod_error;
}

// Consulta password del usuario pedido
int consulta_password(Usuario* lista, int num, char* password)
{
	int posicion = -1;
	int cod_error = 0;
	int i;

	posicion = posicion_password(lista, num, password);
	if (posicion == -1)
	{
		for (i = 0; i < num && posicion == -1; i++)
		{
			if (strcmp(password, lista[i].password) == 0)
			{
				printf("No existe ningun usuario con ese password\n");
				cod_error = -1;
			}
		}
	}

	return cod_error;
}

// Determina la posición de un usuario 
int posicion_usuario(Usuario* lista, int num, char* username)
{
	int i, posicion = -1;

	for (i = 0; i < num && posicion == -1; i++)
		if (strcmp(username, lista[i].username) == 0)
			posicion = i;

	return posicion;
}

// Determina la posición del password 
int posicion_password(Usuario* lista, int num, char* password)
{
	int i, posicion = -1;

	for (i = 0; i < num && posicion == -1; i++)
		if (strcmp(password, lista[i].password) == 0)
			posicion = i;

	return posicion;
}

// Añade un nuevo usuario
Usuario* alta_usuario(Usuario* lista, int* num)
{
	int numero = *num;
	Usuario* lista_old;
	char intro;

	lista_old = lista; // Se guarda la dirección de la lista original por si falla realloc
	if (*num == 0) // Si no hay usuarios aún 
		lista = (Usuario*)malloc(sizeof(Usuario)); // Se guarda memoria del tamaño de Usuario
	else
		lista = (Usuario*)realloc(lista, sizeof(Usuario) * (numero + 1)); // Pide memoria nueva con copia de datos

	if (lista == NULL)
	{
		lista = lista_old;
		printf("Memoria insuficiente para añadir un nuevo usuario\n");
	}
	else
	{
		printf("=====================\n");
		printf("Nombre nuevo usuario: ");
		intro = getchar(); // Eliminamos el intro que estará en el buffer tras la selección del menú
		gets_s((lista + numero)->nombre, LONG_CAD);
		printf("Apellidos nuevo usuario: ");
		gets_s((lista + numero)->apellidos, LONG_CAD);
		printf("Username nuevo de usuario: ");
		gets_s((lista + numero)->username, LONG_CAD);
		printf("Password nuevo usuario: ");
		gets_s((lista + numero)->password, LONG_CAD);
		*num = numero + 1;
		printf("=====================\n");
	}

	escribir_fichero_usuarios(lista, num);

	return lista;
}

// Da de baja un usuario
int baja_usuario(Usuario* lista, int* num)
{
	char username[LONG_CAD], respuesta[2], intro;
	int encontrado = NO;
	int i, j;
	int n = *num;
	int cod_error = 0;

	if (n == 0)
	{
		printf("La lista de usuarios esta vacia\n");
		cod_error = -1;
	}
	else
	{
		printf("Introduzca usuario a eliminar:");
		intro = getchar();
		gets_s(username, LONG_CAD);
		for (i = 0; i < n && encontrado == NO; i++)
			if (strcmp(username, lista[i].username) == 0)
			{
				printf("Datos del usuario encontrado:\n");
				listado_usuarios(lista, n);
				printf("Desea borra este usuario (S/N): ");
				gets_s(respuesta, 2);
				if (strcmp(respuesta, "S") == 0 || strcmp(respuesta, "s") == 0)
				{
					encontrado = SI;
					for (j = i; j < n - 1; j++)
						lista[j] = lista[j + 1];
					*num = n - 1;
				}
				else
					cod_error = -1;
			}
		if (encontrado == NO)
		{
			printf("No se ha encontrado el usuario\n");
			cod_error = -1;
		}
	}
	return cod_error;
}

// Traslada los usuarios de un fichero a memoria
Usuario* leer_fichero_usuarios(int* num)
{
	Usuario* lista = NULL; // Lista con los usuarios obtenidos del fichero
	FILE* fichero; // Descriptor del fichero
	errno_t err; // Código de error del proceso de apertura del fichero
	int i;
	char intro[2], * p; // Para procesar

	err = fopen_s(&fichero, "Usuarios.txt", "r"); // Apertura del fichero solo para lectura
	if (err == 0) // Si no hay error
	{
		fscanf_s(fichero, "%d", &num); // Leemos el número de usuarios que hay en el fichero
		lista = (Usuario*)malloc((*num) * sizeof(Usuario)); // Solicitamos memoria para los datos de los usuarios
		if (lista == NULL) // Si no hay memoria suficiente
			printf("Memoria insuficiente durante la lectura del fichero de usuarios\n");
		else // Si hay memoria suficiente
		{
			fgets(intro, 2, fichero); // Saltamos el intro que hay tras el número (Ascii 10 y 13)
			for (i = 0; i < *num; i++) // Para cada usaurio del fichero
			{

				fgets((lista + i)->nombre, LONG_CAD, fichero); // Leemos el nombre
				p = strchr((lista + i)->nombre, '\n'); // Localizamos el \n del nombre
				*p = '\0'; // Lo cambiamos por un \0
				fgets((lista + i)->apellidos, LONG_CAD, fichero); // Leemos los apellidos
				p = strchr((lista + i)->apellidos, '\n'); // Localizamos el \n de los apellidos
				*p = '\0'; // Lo cambiamos por un \0
				fgets((lista + i)->username, LONG_CAD, fichero); // Leemos el username
				p = strchr((lista + i)->username, '\n'); // Localizamos el \n del username
				*p = '\0'; // Lo cambiamos por un \0
				fgets((lista + i)->password, LONG_CAD, fichero); // Leemos el password
				p = strchr((lista + i)->password, '\n'); // Localizamos el \n del password
				*p = '\0'; // Lo cambiamos por un \0
			}

			fclose(fichero);
		}
	}
	else // Si se ha producido un error en la lectura del fichero de usuarios
		*num = 0;

	return lista;
}

// Traslada los usuarios de memoria a fichero
int escribir_fichero_usuarios(Usuario* lista, int* num)
{
	int i;
	FILE* fichero;
	errno_t err;

	err = fopen_s(&fichero, "Usuarios.txt", "wt"); // Apertura del fichero para escritura (añade datos sin borrar los que ya existían)
	if (err == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", *num); // Se graba en el fichero el número de usuarios
		for (i = 0; i < *num; i++)
		{
			fprintf(fichero, "%s\n", (lista + i)->nombre);
			fprintf(fichero, "%s\n", (lista + i)->apellidos);
			fprintf(fichero, "%s\n", (lista + i)->username);
			fprintf(fichero, "%s\n", (lista + i)->password);
		}

		if (fclose(fichero) != NULL)
			printf("Error en el cierre del archivo\n");


	}
	else
		printf("Se ha producido un error a la hora de grabar el fichero de usuarios\n");

	return err;
}

void guarda_partida(int tablero[][COLUMN])
{
	FILE* fichero;
	int i, j;
	errno_t err;

	err = fopen_s(&fichero, "Partida.txt", "w");
	if (err == 0)
	{
		for (i = 0; i < FILAS; i++)
		{
			for (j = 0; j < COLUMN; j++)
			{
				fprintf(fichero, "%d ", tablero[i][j]);
			}

			fprintf(fichero, "\n");
		}

		fclose(fichero);
	}
	else
		printf("Error en la apertura del fichero\n");

}

void reaunudar_partida(int tablero[][COLUMN])
{
	FILE* fichero;
	int i, j;
	errno_t err;

	err = fopen_s(&fichero, "Partida.txt", "r");
	if (err == 0)
	{
		while (feof(fichero) == NULL)
		{
			for (i = 0; i < FILAS; i++)
			{
				for (j = 0; j < COLUMN; j++)
				{
					fscanf_s(fichero, "%d\n", &tablero[i][j]);
					printf("|%d ", tablero[i][j]);
				}

				printf("|\n");
			}
		}
	}

}

int puntuaciones_jugador1(Usuario* lista, int num, int tablero[][COLUMN], int* fin)
{
	FILE* fichero;
	errno_t err;
	int score = 0;

	if (*fin == 0)
	{
		score += 10;
	}
	else
	{
		if (*fin == 1)
		{
			score += 25;
		}
		else
		{
			if (*fin == 2)
			{
				score -= 10;
			}
		}

	}

	err = fopen_s(&fichero, "Puntuaciones.txt", "wt+");

	if (err == 0)
	{
		fscanf_s(fichero, "%s\n", &lista->username);
		fscanf_s(fichero, "%d\n", &score);

		if (fichero == NULL)
		{
			fprintf(fichero, "%s\n", lista->username);
			fprintf(fichero, "%d\n", score);
		}
		else
		{
			score += score;

			fprintf(fichero, "%s\n", lista->username);
			fprintf(fichero, "%d\n", score);

		}

		if (fclose(fichero) != NULL)
			printf("Error en el cierre del archivo\n");
	}
	else
		printf("Error en la apertura del fichero\n");


	return err;

}

int puntuaciones_jugador2(Usuario* lista, int jugador, int tablero[][COLUMN], int* fin)
{
	FILE* fichero;
	errno_t err;
	int score = 0;

	if (fin == 0)
	{
		score += 10;
	}
	else
	{
		if (*fin == 1)
		{
			score -= 10;
		}
		else
		{
			if (*fin == 2)
			{
				score += 25;
			}
		}

	}

	err = fopen_s(&fichero, "Puntuaciones.txt", "wt+");

	if (err == 0)
	{
		fscanf_s(fichero, "%s\n", &lista->username);
		fscanf_s(fichero, "%d\n", &score);

		if (fichero == NULL)
		{
			fprintf(fichero, "%s\n", lista->username);
			fprintf(fichero, "%d\n", score);
		}
		else
		{
			score += score;

			fprintf(fichero, "%s\n", lista->username);
			fprintf(fichero, "%d\n", score);

		}

		if (fclose(fichero) != NULL)
			printf("Error en el cierre del archivo\n");
	}
	else
		printf("Error en la apertura del fichero\n");

	return err;
}

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

