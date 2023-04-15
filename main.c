#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "src/hash.h"
#include "src/cajas.h"
#include "src/inventario.h"

const int CARGAR_OTRA_LETRA = 0;
const int ERROR_PARAMETROS_NULOS = -1;
const int ERROR_CARGANDO_PARAMETROS = -2;

//Pre: -
//Post: -
void mostrar_opciones()
{
	printf("\nSeleccione una de la siguientes opciones:\n");
	printf("Presione (i) para mostrar el inventario: mostrar un listado de cajas disponibles y la cantidad de pokemones de cada caja\n");
	printf("Presione (c) para cargar otra caja: Pide un nombre de archivo e intenta cargar otra caja\n");
	printf("Presione (m) para combinar dos cajas: Pide dos nombres de cajas y un nombre para el archivo. Combina las cajas y las guarda en el nuevo archivo\n");
	printf("Presione (d) para mostrar la caja: Pide un nombre de caja y muestra el contenido de la misma por pantalla\n");
	printf("Presione (b) para buscar en una caja: Pide un nombre de pokemon, muetras la lista de cajas disponibles y en las que se encuentra ese pokemon\n");
	printf("Presione (q) para salir del programa\n");
}

//Pre: -
//Post: -
void utilizar_menu(inventario_t *inventario_de_cajas)
{
	if(!inventario_de_cajas)
		return;

	mostrar_opciones();
	
	char letra = ' ';
	printf("Ingrese una letra:\n");
	scanf(" %c", &letra);

	int opciones_menu = abrir_menu(inventario_de_cajas, letra);
	
	while(opciones_menu == CARGAR_OTRA_LETRA){
		mostrar_opciones();

		scanf(" %c", &letra);
		opciones_menu = abrir_menu(inventario_de_cajas, letra);
	}
	if(opciones_menu == ERROR_PARAMETROS_NULOS)
		printf("No se ha podido abrir el menu debido a que uno de los parametros recibidos es nulo\n");

	printf("\n El programa finalizo \n");

	return;
}

int main( int argc, char* argv[])
{
	if(argc < 2 || !argv)
		return ERROR_PARAMETROS_NULOS;

	inventario_t *inventario_cajas = inventario_crear(argc);
	if(!inventario_cajas)
		return ERROR_CARGANDO_PARAMETROS;

	inventario_cajas = llenar_inventario(inventario_cajas, argc, argv);
	if(!inventario_cajas){
		inventario_destruir(inventario_cajas);
		return ERROR_CARGANDO_PARAMETROS;
	}

	utilizar_menu(inventario_cajas);
	inventario_destruir(inventario_cajas);

	return 0;
}