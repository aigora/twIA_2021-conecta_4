#include <stdio.h>
#include <malloc.h>

// Funciones prototipo
int menu_principal(void);
void configura(void);
void un_jugador(void);
FILE* crear_fichero(void);

int main(void)
{
 FILE* fichero; // Declaración de la variable de tipo fichero
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
		scanf_s("%d", &orden);
		c = 0;
		do
		{
			tablero[c][orden] = columna;
			c++;
		} while (columna != 0);
		tablero[columna][orden] = 1;
		c = 0;
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
