#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <malloc.h>
#include <string.h>

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
void un_jugador(int[][COLUMN]); 
void dos_jugadores(int[][COLUMN]);
void meter_ficha(int[][COLUMN], int, int*);
void IA(int[][COLUMN], int, int*);
int conecta(int, int, int, int[][COLUMN]);
int contar_vertical(int, int, int, int[][COLUMN]);
int contar_horizontal(int, int, int, int[][COLUMN]);
int contar_diagonal_ascendente(int, int, int, int[][COLUMN]);
int contar_diagonal_descendente(int, int, int, int[][COLUMN]);
int fichero_partida(int, int[][COLUMN]);

// Funciones de gestión de usuarios
Usuario* leer_fichero_usuarios(int*);
int escribir_fichero_usuarios(Usuario*, int);
Usuario* gestion_usuarios(Usuario*, int*);
void listado_usuarios(Usuario*, int);
Usuario* alta_usuario (Usuario*, int*);
int baja_usuario(Usuario*, int*);
int consulta_usuario(Usuario*, int);
int posicion_usuario(Usuario*, int, char*);

int main(void)
{
 int opc; // Opción del menú principal
 int num_usuarios; // Cantidad de usuarios actual
 Usuario* usuarios; // Lista de usuarios actual
 int tablero[FILAS][COLUMN];
	
 // Leer fichero con los usuarios
 usuarios = leer_fichero_usuarios(&num_usuarios); // Traslada los usuarios desde un fichero a memoria
 
 // Bucle principal de la aplicación
 do
 {
	opc = menu_principal();
	int i, tablero[FILAS][COLUMN];
	switch (opc)
	{
	case 1:
			usuarios = gestion_usuarios(usuarios, &num_usuarios);
			break;	
	case 2:
			alta_usuario (usuarios, &num_usuarios);
			un_jugador(tablero);
			break;
		case 3:
			for (i = 0; i < 2; i++)
				alta_usuario (usuarios, &num_usuarios);
			dos_jugadores(tablero);
			break;
		case 4:
			break;
		case 5:
			break;
		}
	} while (opc != 5);
	
   fichero_partida(opc, tablero); // Traslada los movimientos de la partida a un fichero
	
  // Tareas de desconexión y cierre
  escribir_fichero_usuarios(usuarios, num_usuarios); // Traslada los usuarios desde memoria a un fichero
}

int menu_principal(void)
{
 int opcion;
 
 do
 {
    printf("Menu principal Conecta-4\n");
    printf ("========================\n");
    printf("1 - Gestion de usuarios\n");
    printf("2 - 1 Jugador\n");
    printf("3 - 2 Jugadores\n"); 
    printf("4 - Reaunudar partida\n");
    printf("5 - Ver puntuaciones\n");
    printf("6 - Salir\n");
    printf ("Seleccione una opcion: ");
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

void un_jugador(int tablero[][COLUMN]) {
	int turno = 1;
	int fin=0;
	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	printf("Ha seleccionado modo 1 jugador\n");
	printf("Inicio de la partida:\n");
	inicializar_tablero(tablero);//Inicializamos tablero
	imprimir_tablero(tablero);
	printf("\n");
	do {
		printf("\nTURNO %d", turno); //Indica cuantos turnos llevan
		printf("\n");
		printf("Inicio del turno del jugador\n");
		meter_ficha(tablero, 1, &fin); //El jugador mete ficha
		imprimir_tablero(tablero);
		if (fin == 0) { //El jugador 2 solo puede meter ficha si el jugador 1 no ha ganado
			printf("Inicio del turno de la CPU\n");
			IA(tablero, 2, &fin);
			imprimir_tablero(tablero);
			turno++;
		}
	} while (fin == 0);
}

void dos_jugadores(int tablero[][COLUMN]) {
	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	int fin = 0;
	printf("Ha seleccionado modo 2 jugadores\n");
	printf("Inicio de la partida:\n");
	inicializar_tablero(tablero);//Inicializamos tablero
	imprimir_tablero(tablero);
	printf("\n");
	do  // el bucle de turnos continúa hasta que se detecte alguna jugada como victoria para un jugador
	{
		printf("Inicio del turno del jugador 1\n");
		meter_ficha(tablero, 1, &fin);//El jugador elige dónde poner la ficha
		imprimir_tablero(tablero);
		if (fin == 0) // la ficha del seundo jugador solo se puede introducir si no ha ganado el jugador 1
		{
			printf("Inicio del turno del jugador 2\n");
			meter_ficha(tablero, 2, &fin);//El jugador elige dónde poner la ficha
			imprimir_tablero(tablero);
		}
	} while (fin == 0);
}

void meter_ficha(int tablero[][COLUMN], int jugador, int *fin)//Pone la ficha del jugador "jugador" em el tablero
{
	int fila, columna, exito = 0; // declaramos las variables
	do //repetimos la funcion hasta que el usuario introduzca una columna válida
	{
		printf("Introduzca columna:\n");
		scanf_s("%d", &columna); // guardamos la columna en la que el usuario desea introducir la ficha
		columna--; // ajustamos columna a un valor inferior para que las columnas sean del 1 al 7, más intuitivo para el usuario
		if (columna < 0 || columna > (COLUMN - 1))
			printf("Columna no valida-limites 1 a %d-\n", COLUMN - 1);
		else
		{
			if (tablero[0][columna] != 0) {//Si la columna esta completa
				printf("Columna no valida-esta completa-\n");
				columna = -1;//Truco para forzar el nuevo bucle
			}
		}
	} while (columna < 0 || columna > (COLUMN - 1));
		for (fila = FILAS - 1;fila >= 0 && exito == 0;fila--) {//Buscamos fila con un hueco en la columna
			if (tablero[fila][columna] == 0) {//Si está vacía
				tablero[fila][columna] = jugador;//Ponemos ficha del jugador
				exito = 1;
			        if (conecta(fila, columna, jugador, tablero) != 0) { //Comprueba si el jugador puede ganar
				    printf("Ha ganado el jugador %d\n", jugador); //Indica quien es el jugador que ha ganado
				    *fin = jugador;				
			}
		}
	
}
void IA(int tablero[][COLUMN], int jugador, int *fin)
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

	return contador; // Hay que restar uno al contador porque has contado dos veces la ficha
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
	// La diagonal ascendente tiene esta forma: /

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
	// La diagonal descendiente tiene esta forma: \

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
		printf("3 - Consulta usuario\n");
		printf("4 - Listado de los usuarios\n");
		printf("5 - Volver al menu principal\n");
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
			consulta_usuario(lista, *num);
		case 4: 
			listado_usuarios(lista, *num);
			break;
		case 5: 
			break;
		default: printf("Opcion inexistente");
		}
	 } while (opcion != 5);
		       
	return lista;
}

// Muestra la lista de usuarios
void listado_usuarios(Usuario *lista,int numero)
{
	int i;
	
	if (numero == 1)
		printf("No hay usuarios actualmente (excepto la IA)\n");
	else
	{
		printf("En este momento existen %d usuarios %c\n", numero - 1, (numero > 2) ? 's' : ' ');
		printf("Nombre\t\tApellidos\t\tUsername\t\tPassword\n");
		printf("========\t\t========\t\t========\t\t========\n");
		
		for (i = 1; i < numero; i++)
			printf("%s\t\t\t%s\t\t\t%s\t\t\t%s\n",(lista + i)->nombre, (lista + i)->apellidos, (lista + i)->username, (lista + i)->password);
	}
	
	printf("\n\n");
}

// Consulta los datos del usuario a pedir
int consulta_usuario(Usuario* lista, int num)
{
	char username[LONG_CAD];
	int posicion = -1;
	int cod_error = 0;

	printf("Introduzca username a buscar:");
	gets_s(username, LONG_CAD);
	posicion = posicion_usuario(lista, num, username);
	if (posicion == -1)
	{
		printf("No existe ningun usuario con ese username\n");
		cod_error = -1;
	}
	else
	{
		listado_usuarios(lista, num);

	}
	return cod_error;
}

// Determina la posición de un usuario 
int posicion_usuario(Usuario* lista, int num, char* username)
{
	int i, posicion = -1;

	for (i = 0; i < num && posicion == -1; i++)
		if (strcmp(username, lista[i]->username) == 0)
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
	if (*num == 1) // Si no hay usuarios aún (excepto la IA)
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
		printf("Nombre nuevo usuario: ");
		intro = getchar(); // Eliminamos el intro que estará en el buffer tras la selección del menú
		gets_s((lista + numero)->nombre, LONG_CAD);
		printf("Apellidos nuevo usuario: ");
		gets_s((lista + numero)->apellidos, LONG_CAD);
		printf("Username nuevo de usuario: ");
		intro = getchar(); // Eliminamos el intro que estará en el buffer tras la selección del menú
		gets_s((lista + numero)->username, LONG_CAD);
		printf("Password nuevo usuario: ");
		gets_s((lista + numero)->password, LONG_CAD);
		*num = numero + 1;	
	}
	
	escribir_fichero_usuarios(lista, numero);
	
        return lista;
}

// Da de baja un usuario
int baja_usuario(Usuario* lista, int* num)
{
	char username[LONG_CAD], respuesta[2];
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
		gets_s(username, LONG_CAD);
		for (i = 0; i < n && encontrado == NO; i++)
			if (strcmp(username, lista[i]->username) == 0)
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
	char intro[2], *p; // Para procesar
	
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
			for (i = 1; i < *num; i++) // Para cada usaurio del fichero
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
		*num = 1;
	
	return lista;
}
		       
// Traslada los usuarios de memoria a fichero
int escribir_fichero_usuarios(Usuario* lista, int numero)
{
	int i;
	FILE* fichero;
	errno_t err;
	
	err = fopen_s(&fichero, "Usuarios.txt", "a"); // Apertura del fichero para escritura (añade datos sin borrar los que ya existían)
	if (err == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", numero); // Se graba en el fichero el número de usuarios
		fprintf(fichero, "IA\n"); // Se guarda por defecto un usuario llamado IA
		fprintf(fichero, "IA\n"); // El password por defecto será IA
		for (i = 0; i < numero; i++)
		{
			fprintf(fichero, "%d\n", i); // Se numeran los usuarios
			fprintf(fichero, "%s\n", (lista + i)->nombre);
			fprintf(fichero, "%s\n", (lista + i)->apellidos);
			fprintf(fichero, "%s\n", (lista + i)->username);
			fprintf(fichero, "%s\n", (lista + i)->password);
		}
		
		fclose(fichero);
	}
	else
		printf("Se ha producido un error a la hora de grabar el fichero de usuarios\n");
	
	return err;
}




			       
			       
			       
		      
		       
