#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/hash.h"
#include "src/inventario.h"
#include "src/cajas.h"
#include "pa2m.h"

void creacion_de_inventario()
{
	inventario_t *inventario = inventario_crear(2);

	pa2m_afirmar(inventario_crear(0) == NULL, "No puedo crear un inventario con capacidad cero");
	pa2m_afirmar(cantidad_pokemones(NULL) == 0, "No hay pokemones en un inventario nulo");
	pa2m_afirmar(abrir_menu(NULL, 'Q') == -1, "No puedo abrir un menu si no existe un inventario");
	pa2m_afirmar(inventario != NULL, "El inventario se puede crear correctamente");
	pa2m_afirmar(cantidad_pokemones(inventario) == 0, "La cantidad de pokemones en el inventario recien creado es cero");

	inventario_destruir(inventario);
}

void llenar_el_inventario(int argc, char *argv[]){
	if(!argv || argc <= 0)
		return;

	inventario_t *inventario = inventario_crear(argc);
	if(!inventario)
		return;

	pa2m_afirmar( llenar_inventario(NULL, argc, argv) == NULL, "No puedo llenar un inventario que no existe");
	pa2m_afirmar( llenar_inventario(inventario, 0, argv) == NULL, "No puedo llenar un inventario que no tiene tamaño o el mismo es invalido");
	pa2m_afirmar( llenar_inventario (inventario, argc, NULL) == NULL, "No puedo llenar el inventario si no le doy algo con que llenarlo");
	pa2m_afirmar( llenar_inventario(inventario, argc, argv) != NULL, "La funcion llenar inventario funciona correctamente");
	pa2m_afirmar( cantidad_pokemones(inventario) == 30, "La cantidad de pokemon ubicados en el inventario es la correcta");

	inventario_destruir(inventario);
}

void mezclar_las_cajas(int argc, char *argv[]){
	if(!argv || argc <= 0)
		return;

	inventario_t *inventario = inventario_crear(argc);
	if(!inventario)
		return;
	inventario = llenar_inventario(inventario, argc, argv);

	pa2m_afirmar( mezclar_cajas(NULL, "pokemon1.csv", "pokemon2.csv", "nuevo_nombre") == NULL, "No puedo mezclar cajas de un inventario que no existe");
	pa2m_afirmar( mezclar_cajas(inventario, NULL, "pokemon2.csv", "nuevo_nombre") == NULL, "No puedo mezclar cajas si no tengo una de ellas (la primera)");
	pa2m_afirmar( mezclar_cajas(inventario, "pokemon1.csv", NULL, "nuevo_nombre") == NULL, "No puedo mezclar cajas si no tengo una de ellas (la segundo)");
	pa2m_afirmar( mezclar_cajas(inventario, "pokemon1.csv", "pokemon2.csv", NULL) == NULL, "No puedo crear una nueva caja si no le doy un nombre a la misma");

	inventario_destruir(inventario);

}

void abrir_el_menu(){

	inventario_t *inventario = inventario_crear(1);
	pa2m_afirmar(abrir_menu(NULL, 'Q') == -1,"No se puedo abrir el menu de un inventario que no existe");
	pa2m_afirmar(abrir_menu(inventario, 'Q') == 1, "El menu funciona correctamente");

	inventario_destruir(inventario);
}

void cargar_y_guardar_archivos(int argc, char *argv[]){
	
	if(!argv)
		return;

	caja_t *caja_uno = caja_cargar_archivo(argv[1]);
	int archivos_leidos = caja_guardar_archivo(caja_uno, "salida");

	pa2m_afirmar( caja_cargar_archivo("NombreInvalido") == NULL, "No puedo cargar un archivo que no existe");
	pa2m_afirmar( caja_uno != NULL, "Si el archivo existe, me devuelve una caja cargada con los datos del mismo");
	pa2m_afirmar( caja_guardar_archivo(NULL, "salida") == 0, "No puedo guardar un archivo con una caja que no existe");
	pa2m_afirmar( caja_guardar_archivo(caja_uno, NULL) == 0, "No puedo guardar un archivo si no le envio el nombre");
	pa2m_afirmar( archivos_leidos == caja_cantidad(caja_uno), "La funcion guardar archivo funciona correctamente");

	caja_destruir(caja_uno);

}

void combinacion_de_cajas(int argc, char *argv[]){

	caja_t *caja_uno = caja_cargar_archivo(argv[1]);
	caja_t *caja_dos = caja_cargar_archivo(argv[2]);
	caja_t *caja_combinada = caja_combinar(caja_uno, caja_dos);

	pa2m_afirmar( caja_combinar(NULL, caja_dos) == NULL, "No puedo combinar dos cajas si una no existe (la primera)");
	pa2m_afirmar( caja_combinar(caja_uno, NULL) == NULL, "No puedo combinar dos cajas si una no existe (la segunda)");
	pa2m_afirmar( caja_combinada != NULL, "Si las dos cajas existe, puedo combinarlas correctamente");
	pa2m_afirmar( caja_cantidad(caja_combinada) == (caja_cantidad(caja_uno) + caja_cantidad(caja_dos)), "La cantidad de pokemon es la suma de las dos cajas");

	caja_destruir(caja_uno);
	caja_destruir(caja_dos);
	caja_destruir(caja_combinada);

}

void obtener_pokemon_n(int argc, char *argv[]){
	if(!argv)
		return;

	caja_t* caja = caja_cargar_archivo(argv[1]);
	pokemon_t *pokemon = NULL;

	pa2m_afirmar(caja_obtener_pokemon(caja, -1) == NULL, "No puedo obtener un pokemon de una posicion invalida");
	pa2m_afirmar(caja_obtener_pokemon(NULL, 0) == NULL, "No puedo obtener un pokemon de una caja que no existe");
	
	pokemon = caja_obtener_pokemon(caja, 0);
	pa2m_afirmar(strcmp(pokemon_nombre(pokemon),"Articuno") == 0, "Caja obtener pokemon devuelve el pokemon correcto (el primero)");

	pokemon = caja_obtener_pokemon(caja, 5);
	pa2m_afirmar(strcmp(pokemon_nombre(pokemon), "Golbazor") == 0, "Caja obtener pokemon devuelve el pokemon correcto (el quinto)");

	pokemon = caja_obtener_pokemon(caja, 14);
	pa2m_afirmar(strcmp(pokemon_nombre(pokemon), "Squirtle") == 0, "Caja obtener pokemon devuelve el pokemon correcto (el ultimo)");

	pokemon = caja_obtener_pokemon(caja, 50);
	pa2m_afirmar(pokemon_nombre(pokemon) == NULL, "No puedo obtener un pokemn si la posicion supera a la cantidad de pokemones");

	caja_destruir(caja);
}

void aplicar_pokemon(pokemon_t *pokemon){
	if(!pokemon)
	return;
}

void recorrer_cajas(int argc, char *argv[]){
	if(!argv)
		return;

	caja_t* caja = caja_cargar_archivo(argv[1]);
	pa2m_afirmar( caja_recorrer(NULL, aplicar_pokemon) == 0, "No puedo recorrer una caja que no existe");
	pa2m_afirmar( caja_recorrer(caja, NULL) == 0, "No puedo recorrer una caja si no le envio una funcion");
	pa2m_afirmar( caja_recorrer(caja, aplicar_pokemon) == caja_cantidad(caja), "Caja recorrer aplica la funcion enviada a todos los pokemones");

	caja_destruir(caja);
}

int main(int argc, char* argv[])
{
	
	pa2m_nuevo_grupo("PRUEBAS DE INVENTARIO:");

	pa2m_nuevo_grupo("Pruebas de creacion de inventario");
	creacion_de_inventario();

	pa2m_nuevo_grupo("Pruebas de llenar inventario");
	llenar_el_inventario(argc, argv);

	pa2m_nuevo_grupo("Pruebas mezclas cajas");
	mezclar_las_cajas(argc, argv);

	pa2m_nuevo_grupo("Pruebas de abrir menu");
	abrir_el_menu();

	printf("\n\n");
	pa2m_nuevo_grupo("PRUEBAS DE CAJAS POKEMON:");

	pa2m_nuevo_grupo("Pruebas cargar y guardar archivos");
	cargar_y_guardar_archivos(argc, argv);

	pa2m_nuevo_grupo("Pruebas de combinacion de cajas");
	combinacion_de_cajas(argc, argv);

	pa2m_nuevo_grupo("Pruebas obtener pokemon");
	obtener_pokemon_n(argc, argv);

	pa2m_nuevo_grupo("Pruebas recorrer cajas");
	recorrer_cajas(argc, argv);

	return pa2m_mostrar_reporte();
}
