#include <stdio.h>
#include <Windows.h>
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
void un_jugador(void); 
void meter_ficha(int[][7],int jugador);

// Funciones de gestión de usuarios
Usuario* leer_fichero_usuarios(int*);
int escribir_fichero_usuarios(Usuario*,int);
Usuario* gestion_usuarios(Usuario*,int*);
void listado_usuarios(Usuario*,int);
Usuario* add_usuario (Usuario*,int*);

int main(void)
{
 int opc; // Opción del menú principal
 int num_usuarios; // Cantidad de usuarios actual
 Usuario* usuarios; // Lista de usuarios actual
	
 // Leer fichero con los usuarios
 usuarios = leer_fichero_usuarios(&num_usuarios); // Traslada los usuarios desde un fichero a memoria
 
 // Bucle principal de la aplicación
 do
 {
	opc = menu_principal();
	int tablero[6][7];
	int i, j, c, orden, columna;
	switch (opc)
	{
	case 1:
		usuarios = gestion_usuarios(usuarios,&num_usuarios);
		un_jugador();	
		break;
	case 2:
		// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
		printf("ha seleccionado modo 2 jugadores");
		for (i = 0; i < 6; i++) // en este bucle ponermos el tablero a 0 (casillas vacías)
		{
			for (j = 0; j < 7; j++)
			{
				tablero[i][j] = 0;
			}
		}
		printf("turno del jugador 1:\n");

		break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		}
	} while (opc != 5);
 // Tareas de desconexión y cierre
}

int menu_principal(void)
{
 int opcion;
 
 do
 {
 printf("Seleccione opcion:\n");
 printf("1 - 1 jugador\n");
 printf("2 - 2 Jugadores\n");
 printf("3 - Reaunudar partida\n");
 printf("4 - Ver puntuaciones\n");
 printf("5 - Salir\n");
 scanf_s("%d", &opcion);
 if (opcion < 1 || opcion>5)
 printf("\nOpcion inexistente.\n\n");
 } while (opcion < 1 || opcion>5);
 
 return opcion;
}
void un_jugador(void) {
	int i, j, jugador = 1;
	int tablero[6][7];
	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	printf("Ha seleccionado modo 1 jugador\n");
	printf("Inicio de la partida:\n");
	// en este bucle ponermos el tablero a 0 (casillas vacías)
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			tablero[i][j] = 0;
			printf("|%d ", tablero[i][j]);
		}
		printf("|\n");
	}
	printf("\n");
	printf("Inicio del turno del jugador\n");
	meter_ficha(tablero, jugador);
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			printf("|%d ", tablero[i][j]);
		}
		printf("|\n");
	}
}


void meter_ficha(int tablero[][7],int jugador)
{
	int fila=1, columna, c = 6; // declaramos las variables
	do //repetimos la funcion hasta que el usuario introduzca una columna válida
	{
		printf("Introduzca columna:\n");
		scanf_s("%d", &columna); // guardamos la columna en la que el usuario desea introducir la ficha
		if (columna < 1 || columna >7)
			printf("Columna no valida\n");
		else
		{
			do // barremos la columna hasta encontrar un hueco disponible
			{
				fila = tablero[c][columna-1];
				c--;
			} while (fila != 0);
			if (jugador == 1)  // si la ficha es del jugador 1 la colocamos
				tablero[c+1][columna-1] = 1;
			else  // si la ficha es del jugador 2 la colocamos
				tablero[c+1][columna-1] = 2;
		}
	} while (columna < 1 || columna >7);
}

void escribir_fichero(FILE* fichero)
{
    char* username, * password;
    int n = 20, cierre, i;

    fopen_s(&fichero, "Users.txt", "a+");

    username = (char*)malloc(n * sizeof(char));
    password = (char*)malloc(n * sizeof(char));

    if (username == NULL || password == NULL)
        printf("Error: Memoria insuficiente");
    else
    {
        for (i = 0; i < n; i++)
        {
            fscanf_s(fichero, "%c", &username[i], sizeof(username));
            fscanf_s(fichero, "%c", &password[i], sizeof(password));

            while (!feof(fichero))
            {
                fscanf_s(fichero, "%c", &username, sizeof(username));
                fscanf_s(fichero, "%c", &password, sizeof(password));
            }
        }

        cierre = fclose(fichero);

        if (cierre == EOF)
            printf("Problemas al cerrar\n");
        else
            exit(0);
    }

}

FILE* crear_fichero(void) //Función para crear un fichero
{
    FILE* fichero;
    int cierre;

    fopen_s (&fichero, "Users.txt", "a+"); // Crea un fichero para escribir 
    if (fichero == NULL)
        printf("Error no se ha podido abrir el fichero\n");
    else
    {
        cierre = fclose(fichero); // Cierra el fichero

        if (cierre == EOF)
            printf("Problemas al cerrar\n");// Acciones asociadas a la imposibilidad de cerrar el fichero.
        else
            printf("Todo ok\n");
        // Acciones asociadas una vez cerrado el fichero
    }
    return fichero;
}
