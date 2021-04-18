#include <stdio.h>
#include <malloc.h>

// Funciones prototipo
int menu_principal(void);
void configura(void);
int main(void)
{
 int opc;
 // Tareas de configuración y carga
 configura();
 // Bucle principal de la aplicación
 
 do
 {
 opc = menu_principal();
  
		switch (opc)
		{
		case 1:
		        // las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
			printf("ha seleccionado modo 1 jugadores");
			int tablero[6][7];
			// en este bucle ponermos el tablero a 0 (casillas vacías)
			for (i = 0; i < 6; i++)
			{
				for (j = 0; j < 7; j++)
				{
					tablero[i][j] = 0
				}
			}
			break;
		case 2:
			// las casillas con 0 representan casilla vacía, casilla con 1 representa ocupada por ficha del jugador 1 y 2 ocupada por el segundo jugador
			printf("ha seleccionado modo 2 jugadores");
			int tablero[6][7];
			int i,j,c,orden,columna;
			for (i = 0;i<6; i++) // en este bucle ponermos el tablero a 0 (casillas vacías)
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
 printf("Seleccione opción:");
 printf("1 - 1 jugador\n");
 printf("2 - 2 Jugadores\n");
 printf("3 - Reaunudar partida\n");
 printf("4 - Ver puntuaciones\n");
 printf("5 - Salir\n");
 scanf_s("%d", &opcion);
 if (opcion < 1 || opcion>5)
 printf("\nOpción inexistente.\n\n");
 } while (opcion < 1 || opcion>5);
 
 return opcion;
}

void configura(void)
{
 // Establece juego de caracteres castellano
 // Para que funcione hay que partir de un proyecto vacío
 // No utilice la plantilla Aplicación de consola C++
 setlocale(LC_ALL, "spanish");
}