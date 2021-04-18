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
 case 1:break;
 case 2:break;
 case 3:break;
 case 4:break;
 case 5:break;
 case 6:break;
 }
 } while (opc != 6);
 // Tareas de deconexión y cierre
}
int menu_principal(void)
{
 int opcion;
 do
 {
 printf("1 - Titulo opción\n");
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
void configura(void)
{
 // Establece juego de caracteres castellano
 // Para que funcione hay que partir de un proyecto vacío
 // No utilice la plantilla Aplicación de consola C++
 setlocale(LC_ALL, "spanish");
}
