#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <malloc.h>

#define FILAS 6
#define COLUMN 7
#define LONG_CAD 25

typedef struct
{
	char username[LONG_CAD];
	char password[LONG_CAD];
} Usuario;

// Función prototipo
int menu_principal(void);

// Funciones relacionadas con la partida
void inicializar_tablero(int[][COLUMN]);
void imprimir_tablero(int[][COLUMN]);
void un_jugador(int[][COLUMN]); 
void dos_jugadores(int tablero[][COLUMN]);
void meter_ficha(int[][COLUMN], int, int*);
void IA(int[][COLUMN], int);
int conecta(int, int, int, int[][COLUMN]);
int contar_vertical(int, int, int, int[][COLUMN]);
int contar_horizontal(int, int, int, int[][COLUMN]);
int contar_diagonal_ascendente(int, int, int, int[][COLUMN]);
int contar_diagonal_descendente(int, int, int, int[][COLUMN]);

// Funciones de gestión de usuarios
Usuario* leer_fichero_usuarios(int*);
int escribir_fichero_usuarios(Usuario*, int);
Usuario* gestion_usuarios(Usuario*, int*);
void listado_usuarios(Usuario*, int);
Usuario* add_usuario (Usuario*, int*);

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
	int i, j, c, orden, columna;
	switch (opc)
	{
	case 1:
			usuarios = gestion_usuarios(usuarios, &num_usuarios);
			break;	
	case 2:
			add_usuario (usuarios, &num_usuarios);
			un_jugador(tablero);
			break;
		case 3:
			for (i = 0; i < 2; i++)
				add_usuario (usuarios, &num_usuarios);
			dos_jugadores(tablero);
			break;
		case 4:
			break;
		case 5:
			break;
		}
	} while (opc != 5);
	
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
		printf("\nTURNO %d", turno);
		printf("\n");
		printf("Inicio del turno del jugador\n");
		meter_ficha(tablero, 1, &fin);
		imprimir_tablero(tablero);
		if (fin == 0) {
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
		meter_ficha(tablero, 1);//El jugador elige dónde poner la ficha
		imprimir_tablero(tablero);
		fin = conecta(tablero, fin);
		if (fin != 1 && fin != 2) // la ficha del seundo jugador solo se puede introducir si no ha ganado el jugador 1
		{
			printf("Inicio del turno del jugador 2\n");
			meter_ficha(tablero, 2);//El jugador elige dónde poner la ficha
			imprimir_tablero(tablero);
			fin = conecta(tablero, fin);
		}
	} while (fin != 1 && fin != 2);
	printf("ha ganado el jugador %d\n", fin);  // la variable fin recoge que jugador ha conseguido la victoria y se imprime por pantalla
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
			printf("Columna no valida-limites 0 a %d-\n", COLUMN - 1);
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
			}
		}
	if (conecta(fila, columna, jugador, tablero) == 1) {
		printf("Ha ganado el jugador %d\n", jugador);
		*fin = jugador;
	}	
	
}
void IA(int tablero[][COLUMN], int jugador)
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
					columna = COLUMN;// truco para cerrar el bucle for
				}
			}
		}
		if(exito == 0)
		{
			columna = rand() % COLUMN; //Hacemos que la IA elija una columna aleatoria
			for (fila = FILAS - 1; fila >= 0 && exito == 0; fila--)//Buscamos fila con un hueco en la columna
			{
				if (tablero[fila][columna] == 0) {//Si está vacía
						tablero[fila][columna] = jugador;//Ponemos ficha del jugador
						exito = 1;// salimos del bucle while
				}
			}
		}
	} while (exito == 0);
}
int conecta(int fila, int columna, int jugador, int tablero[][COLUMN]) { //Comprueba si hay cuatro en raya
	int a, b, c, d;
	a = contar_vertical(fila, columna, jugador, tablero); //Cuenta en vertical
	b = contar_horizontal(fila, columna, jugador, tablero); //Cuenta en horizontal
	c = contar_diagonal_ascendente(fila, columna, jugador, tablero); //Cuenta en diagonal ascendente 
	d = contar_diagonal_descendente(fila, columna, jugador, tablero); //Cuenta en diagonal descendente
	if (a >= 4 || b >= 4 || c >= 4 || d >= 4) //Si se cumple alguna de las cuatro, hay cuatro en raya
		return 1;
	else
		return 0;
}
int contar_vertical(int fila, int columna, int jugador, int tablero[][COLUMN]) {
	int cont = 0, aux;
	aux = fila;
	//Bucle para contar desde la ultima ficha metida hacia arriba
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		fila--;
	} while (tablero[fila][columna] == jugador && fila >= 0);
	fila = aux; //Volvemos a la posición inicial de la ficha
	//Bucle para contar desde la ultima ficha metida hacia abajo
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		fila++;
	} while (tablero[fila][columna] == jugador && fila < FILAS);
	return cont;
}

int contar_horizontal(int fila, int columna, int jugador, int tablero[][COLUMN]) {
	int cont = 0, aux;
	aux = columna;
	//Bucle para contar desde la ultima ficha metida hacia la izquierda
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna--;
	} while (tablero[fila][columna] == jugador && columna >= 0);
		columna = aux; //Volvemos a la posición inicial de la ficha
		//Bucle para contar desde la ultima ficha metida hacia la derecha
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna++;
	} while (tablero[fila][columna] == jugador && columna < (COLUMN - 1));
	return cont;
}
int contar_diagonal_ascendente(int fila, int columna, int jugador, int tablero[][COLUMN]) {
	int cont = 0, aux1, aux2;
	aux1 = fila;
	aux2 = columna;
	//Bucle para contar desde la ultima ficha metida hacia la izquierda y hacia abajo
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna--;
		fila++;
	} while (tablero[fila][columna] == jugador && columna >= 0 && fila < FILAS);
	fila = aux1; //Volvemos a la posición inicial de la ficha
	columna = aux2; //Volvemos a la posición inicial de la ficha
	//Bucle para contar desde la ultima ficha metida hacia la derecha y hacia arriba
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna++;
		fila--;
	} while (tablero[fila][columna] == jugador && columna < (COLUMN - 1) && fila >= 0);
	return cont;
}
int contar_diagonal_descendente(int fila, int columna, int jugador, int tablero[][COLUMN]) {
	int cont = 0, aux1, aux2;
	aux1 = fila;
	aux2 = columna;
	//Bucle para contar desde la ultima ficha metida hacia la derecha y hacia abajo
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna++;
		fila++;
	} while (tablero[fila][columna] == jugador && columna < (COLUMN - 1) && fila < FILAS);
	fila = aux1; //Volvemos a la posición inicial de la ficha
	columna = aux2; //Volvemos a la posición inicial de la ficha
	//Bucle para contar desde la ultima ficha metida hacia la izquierda y hacia arriba
	do {
		if (tablero[fila][columna] == jugador)
			cont++;
		columna--;
		fila--;
	} while (tablero[fila][columna] == jugador && columna >= 0 && fila >= 0);
	return cont;
}

// Menú con las opciones para gestionar usuarios
Usuario* gestion_usuarios(Usuario* lista, int* num)
{
	int opcion;
	
	do
	{
		printf("Gestion de Usuarios\n");
		printf("===================\n");
		printf("1 - Add usuario\n");
		printf("2 - Listado de los usuarios\n");
		printf("3 - Volver al menu principal\n");
		printf("Seleccione una opcion: ");
		scanf_s("%d", &opcion);
		       
		switch (opcion)
		{
		case 1:
			lista = add_usuario(lista, num);
			break;
		case 2: 
			listado_usuarios(lista, *num);
			break;
		case 3: 
			break;
		default: printf("Opcion inexistente");
		}
	 } while (opcion != 3);
		       
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
		printf("Username\t\tPassword\n");
		printf("========\t\t========\n");
		
		for (i = 1; i < numero; i++)
			printf("%s\t\t\t%s\n", (lista + i)->username, (lista + i)->password);
	}
	
	printf("\n\n");
}
		       
// Añade un nuevo usuario
Usuario* add_usuario(Usuario* lista, int* num)
{
	int numero = *num;
	Usuario* lista_old;
	char intro;
	
	lista_old = lista; // Se guarda la dirección de la lista original por si falla realloc
	if (*num == 1) // Si no hay usuarios aún (excepto la IA)
		lista = (Usuario*)malloc(sizeof(Usuario));
	else
		lista = (Usuario*)realloc(lista, sizeof(Usuario) * (numero + 1)); // Pide memoria nueva con copia de datos
	
	if (lista == NULL)
	{
		lista = lista_old;
		printf("Memoria insuficiente para añadir un nuevo usuario\n");
	}
	else
	{
		printf("Nombre nuevo de usuario: ");
		intro = getchar(); // Eliminamos el intro que estará en el buffer tras la selección del menú
		gets_s((lista + numero)->username, LONG_CAD);
		printf("Password nuevo usuario: ");
		gets_s((lista + numero)->password, LONG_CAD);
		*num = numero + 1;
	}
	
        return lista;
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
		fscanf_s(fichero, "%d", num); // Leemos el número de usuarios que hay en el fichero
		lista = (Usuario*)malloc((*num) * sizeof(Usuario)); // Solicitamos memoria para los datos de los usuarios
		if (lista == NULL) // Si no hay memoria suficiente
			printf("Memoria insuficiente durante la lectura del fichero de usuarios\n");
		else // Si hay memoria suficiente
		{
			fgets(intro, 2, fichero); // Saltamos el intro que hay tras el número (Ascii 10 y 13)
			for (i = 1; i < *num; i++) // Para cada usaurio del fichero
			{
				fgets((lista + i)->username, LONG_CAD, fichero); // Leemos el nombre
				p = strchr((lista + i)->username, '\n'); // Localizamos el \n del nombre
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
			fprintf(fichero, "%s\n", (lista + i)->username);
			fprintf(fichero, "%s\n", (lista + i)->password);
		}
		
		fclose(fichero);
	}
	else
		printf("Se ha producido un error a la hora de grabar el fichero de usuarios\n");
	
	return err;
}


			       
			       
			       
		      
		       
