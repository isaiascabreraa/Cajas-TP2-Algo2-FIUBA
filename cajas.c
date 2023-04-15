#include "hash.h"
#include "abb.h"
#include "cajas.h"
#include "pokemon.h"

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_LINEAS 30
#define MAX_NOMBRE 20
#define FORMATO_OBTENER_NOMBRE "%s;%*[\n]\n"
#define FORMATO_LECTURA_ARCHIVO "%[^\n]\n"
#define FORMATO_ESCRITURA_ARCHIVO "%s;%i;%i;%i\n"

#define FACTOR_DE_CARGA_MAXIMO 75
#define AUMENTAR_CAPACIDAD 2
#define TAMANIO_MINIMO 3

typedef struct buscador_pokemon{
	int posicion_buscada;
	int posicion_actual;
	pokemon_t *pokemon;
}buscador_pokemon_t;

struct _caja_t{
	abb_t* pokemones;
	void (*funcion_pokemon)(pokemon_t*);
	buscador_pokemon_t *datos_pokemon;
};

//Pre: -
//Post: Devuelve un numero real segun el nombre del pokemon sea mayor, menor o igual que el otro. (Compara parar odenar alfabeticamente).
int comparador_pokemones(void *elemento_uno, void *elemento_dos)
{
	if(!elemento_uno || !elemento_dos)
		return -1;

	pokemon_t *pokemon_uno = elemento_uno;
	pokemon_t *pokemon_dos = elemento_dos;

	return strcmp(pokemon_nombre(pokemon_uno), pokemon_nombre(pokemon_dos));
}

//Pre: -
//Post: -
caja_t *llenar_caja(FILE *mis_pokemones, caja_t *caja_pokemon){

	if(!mis_pokemones || !caja_pokemon)
		return NULL;

	char string_pokemon[MAX_LINEAS];
	pokemon_t *pokemon_auxiliar = NULL;

	int leidos = fscanf( mis_pokemones, FORMATO_LECTURA_ARCHIVO, string_pokemon);
	while (leidos == 1) {

		pokemon_auxiliar = pokemon_crear_desde_string(string_pokemon);
		
		if(pokemon_auxiliar != NULL)
			abb_insertar(caja_pokemon->pokemones, pokemon_auxiliar);

		leidos = fscanf( mis_pokemones, FORMATO_LECTURA_ARCHIVO, string_pokemon);
	}
	fclose(mis_pokemones);
	return caja_pokemon;
}

//Pre: Los datos del archivo deben estar separados con ; entre cada campo distinto.
//Post: Devuelve una caja compuesta por los datos que estaban escritos correctamente.
caja_t *caja_cargar_archivo(const char* mi_archivo)
{
	if (!mi_archivo)
		return NULL;

	FILE* mis_pokemones = fopen( mi_archivo, "r");
	if (!mis_pokemones)
		return NULL;

	caja_t *caja_pokemon = calloc(1,sizeof(caja_t));	
	if (!caja_pokemon){
		fclose(mis_pokemones);
		return NULL;
	}

	caja_pokemon->pokemones = abb_crear(comparador_pokemones);
	if(!caja_pokemon->pokemones){
		free(caja_pokemon);
		fclose(mis_pokemones);
		return NULL;
	}

	caja_pokemon = llenar_caja(mis_pokemones, caja_pokemon);
	if(!caja_pokemon)
		caja_destruir(caja_pokemon);

	return caja_pokemon;
}

//Pre: -
//Post: -
void destruir_elemento(void *elemento)
{
	if(!elemento)
		return;
	free(elemento);
}

//Pre: -
//Post: -
void caja_destruir(caja_t* caja)
{
	if (!caja)
		return;
	
	abb_destruir_todo(caja->pokemones, destruir_elemento);
	free(caja);
}

//Pre: -
//Post: -
bool escribir_archivo(void* elemento, void *archivo_a_escribir){

	if(!elemento || !archivo_a_escribir)
		return false;

	FILE* archivo_salida = archivo_a_escribir;
	pokemon_t *pokemon = elemento;

	fprintf(archivo_salida, FORMATO_ESCRITURA_ARCHIVO , pokemon_nombre(pokemon), pokemon_nivel(pokemon), pokemon_ataque(pokemon), pokemon_defensa(pokemon));

	return true;
}

//Pre: -
//Post: Devuelve un archivo csv con el contenido de una de las cajas.
int caja_guardar_archivo( caja_t* caja, const char* mi_caja)
{
	if (!mi_caja || !caja)
		return 0;

	FILE* archivo_salida = fopen(mi_caja, "w");
	if(!archivo_salida)
		return 0;

	size_t pokemones_leidos = 0;
	pokemones_leidos = abb_con_cada_elemento(caja->pokemones, INORDEN, escribir_archivo, archivo_salida);

	fclose(archivo_salida);

	return (int)pokemones_leidos;
}

//Pre: -
//Post: -
int caja_cantidad( caja_t* caja)
{
	if (!caja)
		return 0;
	
	return (int)abb_tamanio(caja->pokemones);
}

//Pre: -
//Post: -
bool combinar_cajas(void *elemento, void *caja_recibida){

	if(!elemento || !caja_recibida)
		return false;

	pokemon_t *pokemon = elemento;
	pokemon_t *pokemon_auxiliar = NULL;
	caja_t *caja_combinada = caja_recibida;

	char lineas_leidas[MAX_LINEAS];

	sprintf( lineas_leidas, FORMATO_ESCRITURA_ARCHIVO, pokemon_nombre(pokemon), pokemon_nivel(pokemon), 
		pokemon_ataque(pokemon), pokemon_defensa(pokemon));

	pokemon_auxiliar = pokemon_crear_desde_string(lineas_leidas);
		
		if(pokemon_auxiliar != NULL)
			abb_insertar(caja_combinada->pokemones, pokemon_auxiliar);

	return true;
}

//Pre: -
//Post: Combina los pokemones de ambas cajas y los devuelve en una nueva sin afectar el contenido de las anteriores.
caja_t* caja_combinar( caja_t* caja1, caja_t* caja2)
{
	if (!caja1 || !caja2)
		return NULL;

	caja_t* caja_combinar = calloc(1, sizeof(caja_t));
	if (!caja_combinar)
		return NULL;

	caja_combinar->pokemones = abb_crear(comparador_pokemones);
	if (!caja_combinar->pokemones) {
		free(caja_combinar);
		return NULL;
	}
	abb_con_cada_elemento(caja1->pokemones, INORDEN, combinar_cajas, caja_combinar);
	abb_con_cada_elemento(caja2->pokemones, INORDEN, combinar_cajas, caja_combinar);

	return caja_combinar;
}

//Pre: -
//Post: -
bool aplicar_funcion_pokemon(void *elemento, void *caja_recibida){
	
	if(!caja_recibida || !elemento)
		return false;

	pokemon_t *pokemon = elemento;
	caja_t *caja = caja_recibida;
	caja->funcion_pokemon(pokemon);

	return true;
}

//Pre: -
//Post: Recorre una caja y le aplica una funcion a cada pokemon en ella.
int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t*))
{
	if (!caja || !funcion)
		return 0;

	caja->funcion_pokemon = funcion;
	
	return (int)abb_con_cada_elemento( caja->pokemones, INORDEN, aplicar_funcion_pokemon, caja);
}

//Pre: -
//Post: -
bool buscar_pokemon_n( void *pokemon_recibido, void *datos_recibidos){

	buscador_pokemon_t *datos_pokemon = datos_recibidos;

	if(datos_pokemon->posicion_actual == datos_pokemon->posicion_buscada){
		datos_pokemon->pokemon = pokemon_recibido;
		return false;
	
	} else {
		datos_pokemon->posicion_actual++;
		return true;	
	}
}

//Pre: -
//Post: -
pokemon_t* caja_obtener_pokemon(caja_t* caja, int n)
{
	if (!caja || n < 0)
		return NULL;

	caja->datos_pokemon = calloc(1, sizeof(buscador_pokemon_t));
	caja->datos_pokemon->posicion_buscada = n;
	pokemon_t *pokemon = NULL;

	abb_con_cada_elemento(caja->pokemones, INORDEN, buscar_pokemon_n, caja->datos_pokemon);
	pokemon = caja->datos_pokemon->pokemon;
	
	free(caja->datos_pokemon);
	return pokemon; 
}