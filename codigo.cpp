#include <stdio.h>
#include <malloc.h>

// Funciones prototipo
int menu_principal(void);
void configura(void);
void un_jugador(void);
void meter_ficha(int[][7],int jugador);
void escribir_fichero(FILE*);
FILE* crear_fichero(void);

int main(void)
{
 FILE* fichero; // Declaración de la variable de tipo fichero
 escribir_fichero(fichero);
 int opc;
 // Tareas de configuración y carga
 configura();
 // Bucle principal de la aplicación
	
 fichero = crear_fichero(); //Llamada a la función crear_fichero
 
 do
 {
	opc = menu_principal();
	int tablero[6][7];
	int i, j, c, orden, columna;
	switch (opc)
	{
	case 1:
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
	int i, j;
	int tablero[6][7];
	// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
	printf("Ha seleccionado modo 1 jugadores\n");
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

}

void configura(void)
{
 // Establece juego de caracteres castellano
 // Para que funcione hay que partir de un proyecto vacío
 // No utilice la plantilla Aplicación de consola C++
 setlocale(LC_ALL, "spanish");
}

void meter_ficha(int tablero[][7],int jugador)
{
	int fila=1, columna, c = 0; // declaramos las variables
	do //repetimos la funcion hasta que el usuario introduzca una columna válida
	{
		printf("introduzca columna:\n");
		scanf_s("%d", &columna); // guardamos la columna en la que el usuario desea introducir la ficha
		if (columna < 0 || columna >6)
			printf("columna no válida\n");
		else
		{
			do // barremos la columna hasta encontrar un hueco disponible
			{
				fila = tablero[c][columna];
				c++;
			} while (fila != 0);
			if (jugador == 1)  // si la ficha es del jugador 1 la colocamos
				tablero[fila][columna] = 1;
			else  // si la ficha es del jugador 2 la colocamos
				tablero[fila][columna] = 2;
		}
	} while (columna < 0 || columna >6);
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
