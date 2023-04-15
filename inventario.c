#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"
#include "hash.h"
#include "lista.h"
#include "cajas.h"
#include "pokemon.h"
#include "inventario.h"

#define MAX_NOMBRE 20

const int ERROR = -1;
const int FINALIZAR = 1;
const int INGRESAR_OTRA_LETRA = 0;

typedef struct directorio{
	hash_t *indice_pokemon;
	lista_t *cajas_recorridas;
}directorio_t;

struct inventario_de_cajas{
	hash_t *caja_contenedora;
	size_t cantidad_pokemones;
	directorio_t *directorio;
};

//Pre: -
//Post: -
void imprimir_pokemon(pokemon_t *pokemon)
{
	if (!pokemon)
		return;

	printf("El nombre del pokemon es: %s\n", pokemon_nombre(pokemon));
	printf("El nivel del pokemon es: %i\n", pokemon_nivel(pokemon));
	printf("El ataque del pokemon es: %i\n", pokemon_ataque(pokemon));
	printf("La defensa del pokemon es: %i\n", pokemon_defensa(pokemon));
	printf("\n");
}

//Pre: -
//Post: -
bool imprimir_pokemones(const char *clave, void *elemento, void *aux)
{
	if (!elemento)
		return false;
	
	pokemon_t *pokemon = elemento;
	imprimir_pokemon(pokemon);
	
	return true;
}

//Pre: -
//Post: -
bool mostrar_cajas_en_inventario(const char *clave, void *elemento, void *aux)
{
	if (!clave || !elemento)
		return false;

	caja_t *caja = elemento;
	printf("\n Nombre de la caja: %s", clave);
	printf(" - Cantidad de pokemones almacenados: %i", caja_cantidad(caja));
	return true;
}

//Pre: -
//Post: -
bool imprimir_localizacion_pokemon(const char * clave, void *nombre_caja, void *aux)
{	
	if (!nombre_caja)
		return false;

	char *nombre_buscado = nombre_caja;
	printf("%s\n", nombre_buscado);
	return true;
}

//Pre: -
//Post: Devuelve un numero real segun el nombre sea mayor, menor o igual que el otro. (Compara parar odenar alfabeticamente).
int comparador_nombres(void *elemento_uno, void *elemento_dos)
{
	if (!elemento_uno || !elemento_dos)
		return ERROR;

	char *nombre_uno = elemento_uno;
	char *nombre_dos = elemento_dos;

	return strcmp(nombre_uno, nombre_dos);
}

//Pre: -
//Post: -
bool actualizar_indice(const char *clave, void *elemento, void *inventario)
{
	if(!clave || !elemento || !inventario)
		return false;

	inventario_t *inventario_cajas = inventario;
	caja_t *caja_actual = elemento;

	if (!lista_buscar_elemento(inventario_cajas->directorio->cajas_recorridas, comparador_nombres, (char*)clave)) {
		for (int i = 0; i < caja_cantidad(caja_actual); i++) {
			
			pokemon_t *pokemon = caja_obtener_pokemon(caja_actual, i);
			if(!pokemon)
				return false;

			hash_t *hash_buscado = hash_obtener(inventario_cajas->directorio->indice_pokemon, pokemon_nombre(pokemon));

			if (!hash_buscado) {
				hash_buscado = hash_crear(0);
				if (!hash_buscado)
					return false;
			}
			hash_insertar(hash_buscado, clave, (char*)clave, NULL);
			inventario_cajas->cantidad_pokemones++;
			hash_insertar(inventario_cajas->directorio->indice_pokemon, pokemon_nombre(pokemon), hash_buscado, NULL);
		}
		lista_insertar(inventario_cajas->directorio->cajas_recorridas, (char*)clave);
	}
	return true;
}

//Pre: -
//Post: -
void *aplicar_opcion_b(directorio_t *directorio)
{
	if (!directorio)
		return NULL;

	char nombre_pokemon[MAX_NOMBRE];
	printf("Escriba el nombre del pokemon buscado\n");
	scanf(" %s", nombre_pokemon);

	hash_t *localizacion_del_pokemon = hash_obtener(directorio->indice_pokemon, nombre_pokemon);
	printf("El pokemon se encuentra en: %i caja(s).\n", hash_cantidad(localizacion_del_pokemon));

	if (!localizacion_del_pokemon)
		printf("No se encuentra en ninguna caja\n");
	else {
		printf("Y las cajas son:\n");
		hash_con_cada_clave(localizacion_del_pokemon, imprimir_localizacion_pokemon, NULL);
	}
	return localizacion_del_pokemon;
}

//Pre: -
//Post: Muestra los pokemones dentro de una caja.
int aplicar_opcion_d(hash_t *caja_contenedora)
{
	if (!caja_contenedora)
		return ERROR;

	char nombre_caja[MAX_NOMBRE];
	printf("Introduza el nombre de la caja \n");
	scanf(" %s", nombre_caja);

	bool caja_existe = hash_contiene(caja_contenedora, nombre_caja);

	while (!caja_existe){
		printf("La caja mencionada no existe, ingrese un nuevo nombre o Q si desea volver atras\n");
		scanf(" %s", nombre_caja);

		if (*nombre_caja == 'Q')
			return ERROR;
		caja_existe = hash_contiene(caja_contenedora, nombre_caja);
	}
	
	caja_t* caja_buscada = hash_obtener(caja_contenedora, nombre_caja);
	if(!caja_buscada)
		return ERROR;

	int elementos_recorridos = caja_recorrer(caja_buscada, imprimir_pokemon);
	return elementos_recorridos;
}

//Pre: -
//Post: Carga una nueva caja al inventario de cajas.
void *aplicar_opcion_c(inventario_t *inventario)
{
	if (!inventario)
		return NULL;

	printf("Introduza el nombre del nuevo archivo a cargar: \n");
	char nombre_archivo[MAX_NOMBRE];
	scanf(" %s", nombre_archivo);

	bool archivo_existe = hash_contiene(inventario->caja_contenedora, nombre_archivo);
	
	while (archivo_existe){
		printf("El archivo ya ha sido cargado anteriormente, ingrese un nuevo nombre o Q si desea volver atras\n");
		scanf(" %s", nombre_archivo);

		if (*nombre_archivo == 'Q')
			return NULL;

		archivo_existe = hash_contiene(inventario->caja_contenedora, nombre_archivo);
	}
	caja_t *caja_auxiliar = caja_cargar_archivo(nombre_archivo);
	if (!caja_auxiliar)
		return NULL;

	hash_insertar(inventario->caja_contenedora, nombre_archivo, caja_auxiliar, NULL);
	hash_con_cada_clave(inventario->caja_contenedora, actualizar_indice, inventario);
	return caja_auxiliar;
}

//Pre: -
//Post: -
void *mezclar_cajas(inventario_t *inventario, char* nombre_uno, char *nombre_dos, char *nombre_nueva_caja)
{
	if (!inventario || !nombre_uno || !nombre_dos || !nombre_nueva_caja)
		return NULL;

	caja_t *caja_uno = hash_obtener(inventario->caja_contenedora, nombre_uno);
	caja_t *caja_dos = hash_obtener(inventario->caja_contenedora, nombre_dos);
	caja_t *caja_combinada = caja_combinar( caja_uno, caja_dos);
	if(!caja_combinada)
		return NULL;
	
	hash_insertar(inventario->caja_contenedora, nombre_nueva_caja, caja_combinada, NULL);
	hash_con_cada_clave(inventario->caja_contenedora, actualizar_indice, inventario);
	
	return caja_combinada;
}

//Pre: -
//Post: Combina dos cajas y las guarda en un archivo. La nueva caja creada se suma al inventario de cajas.
int aplicar_opcion_m(inventario_t *inventario_cajas)
{	
	if (!inventario_cajas)
		return ERROR;

	char nombre_uno[MAX_NOMBRE];
	char nombre_dos[MAX_NOMBRE];
	char nombre_nueva_caja[MAX_NOMBRE];

	printf("\nIntroduza el nombre de la PRIMERA caja que desea combinar:\n");
	scanf(" %s", nombre_uno);

	printf("\nIntroduza el nombre de la SEGUNDA caja que desea combinar:\n");
	scanf(" %s", nombre_dos);

	printf("\nIntroduzca el nombre de la nueva caja\n");
	scanf(" %s", nombre_nueva_caja);
	
	while (hash_contiene(inventario_cajas->caja_contenedora, nombre_nueva_caja)){
		printf("El nombre ya se encuentra utilizado, introduzca otro\n");
		scanf(" %s", nombre_nueva_caja);
	}
	
	caja_t *caja_combinada = mezclar_cajas(inventario_cajas, nombre_uno, nombre_dos, nombre_nueva_caja);
		if (!caja_combinada){
			printf("No se pudieron combinar las cajas\n");
			return ERROR;
		}
	return caja_guardar_archivo(caja_combinada, "salida");
}

//Pre: -
//Post: Imprime por pantalla los nombres de caja una de las cajas y la cantidad de pokemones en cada una.
int aplicar_opcion_i(hash_t *caja_contenedora)
{
	if (!caja_contenedora)
		return ERROR;

	printf("\n Las cajas disponibles son: \n");
	int cajas_recorridas = (int)hash_con_cada_clave(caja_contenedora, mostrar_cajas_en_inventario, NULL);
	printf("\n");

	return cajas_recorridas;
}

//Pre: -
//Post: Devuelve true si el programa debe finalizar o false en caso de error.
int abrir_menu(inventario_t *inventario_de_cajas, char comando)
{	
	if (!inventario_de_cajas || !comando)
		return ERROR;

	comando = (char)toupper(comando);

	if (comando == 'I')
		aplicar_opcion_i(inventario_de_cajas->caja_contenedora);
	else if (comando == 'C')
		aplicar_opcion_c(inventario_de_cajas);
	else if (comando == 'M')
		aplicar_opcion_m(inventario_de_cajas);
	else if (comando == 'D')
		aplicar_opcion_d(inventario_de_cajas->caja_contenedora);
	else if (comando == 'B')
		aplicar_opcion_b(inventario_de_cajas->directorio);
	else if (comando == 'Q')
		return FINALIZAR;
	 else
		printf("\n Ingrese una opcion valida \n");

	return INGRESAR_OTRA_LETRA;
}

//Pre: -
//Post: -
size_t cantidad_pokemones(inventario_t *inventario)
{
	if (!inventario)
		return 0;
	return inventario->cantidad_pokemones;
}

//Pre: -
//Post: -
inventario_t *inventario_crear(int argc)
{
	if (argc <= 0)
		return NULL;

	inventario_t *inventario_cajas = calloc(1, sizeof(inventario_t));
	if (!inventario_cajas)
		return NULL;

	inventario_cajas->directorio = calloc(1, sizeof(directorio_t));
	if (!inventario_cajas->directorio){
		inventario_destruir(inventario_cajas);
		return NULL;
	}
	inventario_cajas->caja_contenedora = hash_crear((size_t)argc);
	if (!inventario_cajas->caja_contenedora){
		inventario_destruir(inventario_cajas);
		return NULL;
	}
	inventario_cajas->directorio->indice_pokemon = hash_crear((size_t)argc);
	if (!inventario_cajas->directorio->indice_pokemon){
		inventario_destruir(inventario_cajas);
		return NULL;
	}
	inventario_cajas->directorio->cajas_recorridas = lista_crear();
	if (!inventario_cajas->directorio->cajas_recorridas){
		inventario_destruir(inventario_cajas);
		return NULL;
	}
	return inventario_cajas;
}

//Pre: -
//Post: -
inventario_t *llenar_inventario(inventario_t *inventario_cajas, int argc, char *argv[])
{
	if (!inventario_cajas || !argv || argc <= 0)
		return NULL;

	for (int i = 0; i < argc; i++) {
		hash_insertar(inventario_cajas->caja_contenedora, argv[i+1], caja_cargar_archivo(argv[i+1]), NULL);
	}
	hash_con_cada_clave(inventario_cajas->caja_contenedora, actualizar_indice, inventario_cajas);

	return inventario_cajas;
}

//Pre: -
//Post: -
bool funcion_eliminar_indice(const char *clave, void *elemento, void *aux)
{
	if (elemento != NULL)
		hash_destruir(elemento);

	return true;
}

//Pre: -
//Post: -
bool funcion_eliminar_cajas(const char *clave, void *elemento, void *aux)
{
	if (elemento != NULL)
		caja_destruir(elemento);

	return true;
}

//Pre: -
//Post: -
void inventario_destruir(inventario_t *inventario_cajas)
{
	if (!inventario_cajas)
		return;

	hash_con_cada_clave(inventario_cajas->directorio->indice_pokemon, funcion_eliminar_indice, NULL);
	hash_con_cada_clave(inventario_cajas->caja_contenedora, funcion_eliminar_cajas, NULL);

	hash_destruir(inventario_cajas->caja_contenedora);
	hash_destruir(inventario_cajas->directorio->indice_pokemon);
	lista_destruir(inventario_cajas->directorio->cajas_recorridas);

	free(inventario_cajas->directorio);
	free(inventario_cajas);
}