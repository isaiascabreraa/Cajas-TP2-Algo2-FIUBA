#include "pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOMBRE_POKEMON 20
#define FORMATO_LECTURA_POKEMON "%[^;];%i;%i;%i\n"

const char* NOMBRE_INVALIDO = " ";
const int NIVEL_INVALIDO = -1;
const int ATAQUE_INVALIDO = -1;
const int DEFENSA_INVALIDA = -1;

struct _pokemon_t {
	char nombre[MAX_NOMBRE_POKEMON];
	int nivel;
	int ataque;
	int defensa;
};

//Pre: -
//Post:
bool es_pokemon_valido( pokemon_t* pokemon)
{
	return( strcmp(pokemon->nombre, NOMBRE_INVALIDO) != 0 || pokemon->nombre != NULL || pokemon->nivel > NIVEL_INVALIDO ||
	 pokemon->ataque > ATAQUE_INVALIDO || pokemon->defensa > DEFENSA_INVALIDA);
}

//Pre: EL string debe respetar el formato de lectura en donde se separan los valores con ;.
//Post: Llena el struc de pokemon_t con los datos que pueda obtener de un string.
pokemon_t *pokemon_crear_desde_string(char *string_pokemones)
{
	if (!string_pokemones)
		return NULL;

	pokemon_t* pokemon = NULL;
	pokemon = malloc(sizeof(pokemon_t));

	if (pokemon == NULL)
		return NULL;

	int leidos_correctamente = sscanf( string_pokemones,FORMATO_LECTURA_POKEMON, pokemon->nombre, &pokemon->nivel, &pokemon->ataque, &pokemon->defensa);
	
	if (leidos_correctamente != 4 || !es_pokemon_valido(pokemon)) {
		free(pokemon);
		return NULL;
	
	}
	return pokemon;
}

//Pre: -
//Post: -
int pokemon_nivel(pokemon_t *pokemon)
{
	if (!pokemon)
		return 0;

	return pokemon->nivel;
}

//Pre: -
//Post: -
int pokemon_ataque(pokemon_t *pokemon)
{
	if (!pokemon)
		return 0;

	return pokemon->ataque;
}

//Pre: -
//Post: -
int pokemon_defensa(pokemon_t *pokemon)
{
	if (!pokemon)
		return 0;

	return pokemon->defensa;
}

//Pre: -
//Post: -
const char* pokemon_nombre(pokemon_t* pokemon)
{
	if (!pokemon)
		return 0;
	
	return pokemon->nombre;
}

//Pre: -
//Post: Libera la memoria asociada al pokemon.
void pokemon_destruir(pokemon_t *pokemon)
{
	if(!pokemon)
		return;

	free(pokemon);
}