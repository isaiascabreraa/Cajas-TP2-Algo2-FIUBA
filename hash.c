#include "hash.h"
#include "pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FACTOR_DE_CARGA_MAXIMO 75
#define AUMENTAR_CAPACIDAD 2
#define TAMANIO_MINIMO 3

typedef struct par {
	char *clave;
	void *valor;
	struct par *par_siguiente;
}par_t;

struct hash {
	par_t **tabla;
	size_t capacidad;
	size_t ocupados;
};

//Pre: -
//Post: Devuelve una copia del string enviado por el usuario.
char *strdup(const char *string)
{
	if (!string)
		return NULL;

	char* string_copia = malloc((1 + strlen(string))*sizeof(char));
	if (!string_copia)
		return NULL;

	strcpy(string_copia, string);
	return string_copia;
}

//Pre: -
//Post: Devuelve un valor numerico segun un string enviado.
size_t funcion_hash(const char *string)
{
	if (!string)
		return 0;

	size_t resultado_hash = 5381;
	int c;

	while ((c = *(string++))) {
		resultado_hash = ((resultado_hash << 5) + resultado_hash + (size_t)c);
	}
	return resultado_hash;
}

//Pre: -
//Post: -
hash_t *hash_crear(size_t capacidad)
{
	size_t capacidad_inicial = capacidad;

	if (capacidad < TAMANIO_MINIMO)
		capacidad_inicial = TAMANIO_MINIMO;

	hash_t *hash_creado = calloc(1,sizeof(hash_t));
	if (!hash_creado)
		return NULL;

	hash_creado->capacidad = capacidad_inicial;

	hash_creado->tabla = calloc(capacidad_inicial, sizeof(par_t*));
	if (!hash_creado->tabla) {
		free(hash_creado);
		return NULL;
	}
	return hash_creado;
}

//Pre: -
//Post: -
par_t *crear_par(hash_t * hash, const char *clave, void *elemento)
{
	if (!clave)
		return NULL;

	par_t *par_creado = calloc(1, sizeof(par_t));
	if (!par_creado)
		return NULL;

	par_creado->clave = strdup(clave);
	par_creado->valor = elemento;

	return par_creado;
}

//Pre: -
//Post: Devuelve el porcentaje de ocupacion de la tabla.
float factor_de_carga(hash_t *hash)
{	
	if (!hash)
		return 0;
	return ((float)hash->ocupados * 100)/(float)hash->capacidad;
}

//Pre: -
//Post: -
hash_t *hash_rehash(hash_t *hash)
{
	if (!hash)
		return NULL;

	hash_t *nuevo_hash = hash_crear((hash->capacidad)*AUMENTAR_CAPACIDAD);
	if (!nuevo_hash)
		return NULL;

	for (size_t i = 0; i < hash->capacidad; i++) {

		par_t *par_auxiliar = NULL;

		if (hash->tabla[i] != NULL) {
			hash_insertar(nuevo_hash, hash->tabla[i]->clave, hash->tabla[i]->valor, NULL);
			par_auxiliar = hash->tabla[i]->par_siguiente;

			while(par_auxiliar != NULL) {
				hash_insertar(nuevo_hash, par_auxiliar->clave, par_auxiliar->valor, NULL);
				par_auxiliar = par_auxiliar->par_siguiente;
			}
		}
	}
	hash_t aux = *hash;
	*hash = *nuevo_hash;
	*nuevo_hash = aux;

	hash_destruir(nuevo_hash);
	return hash;
}

//Pre: -
//Post: -
par_t *recorrer_pares_insertar( hash_t *hash, const char *clave, void *elemento, par_t *par_actual, void **anterior)
{
	if (!hash || !clave)
		return NULL;

	if (par_actual == NULL) {
		par_t *par_nuevo = crear_par(hash, clave, elemento);
		if (!par_nuevo)
			return NULL;
		
		if (anterior != NULL)
			*anterior = NULL;

		hash->ocupados++;
		return par_nuevo;
	}
	else if (strcmp(par_actual->clave, clave) == 0) {
		
		if (anterior != NULL)
			*anterior = par_actual->valor;

		par_actual->valor = elemento;
	}
	else
		par_actual->par_siguiente = recorrer_pares_insertar(hash, clave, elemento, par_actual->par_siguiente, anterior);

	return par_actual;
}

//Pre: -
//Post: -
hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento, void **anterior)
{
	if (!hash || !hash->tabla || !clave)
		return NULL;

	if (factor_de_carga(hash) > FACTOR_DE_CARGA_MAXIMO)
		hash_rehash(hash);
	
	size_t indice = funcion_hash(clave) % hash->capacidad;
	
	hash->tabla[indice] = recorrer_pares_insertar( hash, clave, elemento, hash->tabla[indice], anterior);

	return hash;
}

//Pre: -
//Post: -
par_t *recorrer_pares_quitar( hash_t *hash, const char *clave, par_t *par_actual, void **elemento)
{
	if (!hash || !clave || !par_actual)
		return NULL;

	par_t *par_auxiliar = NULL;

	if (strcmp(par_actual->clave, clave) == 0) {
		
		*elemento = par_actual->valor;
		
		if (par_actual->par_siguiente != NULL)
			par_auxiliar = par_actual->par_siguiente;

		free(par_actual->clave);
		free(par_actual);
		hash->ocupados--;
		return par_auxiliar;
	}
	else
		par_actual->par_siguiente = recorrer_pares_quitar(hash, clave, par_actual->par_siguiente, elemento);

	return par_actual;
}

//Pre: -
//Post: -
void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	void *elemento = NULL;
	size_t indice = funcion_hash(clave) % hash->capacidad;

	if (hash->tabla[indice] != NULL) {

		hash->tabla[indice] = recorrer_pares_quitar(hash, clave, hash->tabla[indice], &elemento);
	}
	return elemento;
}

//Pre: -
//Post: -
void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t indice = funcion_hash(clave) % hash->capacidad;

	void *elemento = NULL;
	par_t *par_auxiliar = NULL;

	if (hash->tabla[indice] != NULL){
		
		if (strcmp(hash->tabla[indice]->clave, clave) != 0) {
			par_auxiliar = hash->tabla[indice]->par_siguiente;

			while (par_auxiliar != NULL){
				
				if (strcmp(par_auxiliar->clave, clave) == 0) {
					elemento = par_auxiliar->valor;
					par_auxiliar = NULL;
				}
				else
					par_auxiliar = par_auxiliar->par_siguiente;
			}
		}
		else
			elemento = hash->tabla[indice]->valor;
	}
	return elemento;
}

//Pre: -
//Post: -
bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t indice = funcion_hash(clave) % hash->capacidad;
	bool existe_clave = false;

	par_t *par_auxiliar = NULL;

	if (hash->tabla[indice] != NULL) {
		
		if (strcmp(hash->tabla[indice]->clave, clave) != 0){
			par_auxiliar = hash->tabla[indice]->par_siguiente;

			while (par_auxiliar != NULL) {
				
				if (strcmp(par_auxiliar->clave, clave) == 0) {
					existe_clave = true;
					par_auxiliar = NULL;
				}
				else
					par_auxiliar = par_auxiliar->par_siguiente;
			}
		}
		else
			existe_clave = true;
	}
	return existe_clave;
}

//Pre: -
//Post: -
size_t hash_cantidad(hash_t *hash)
{
	if (!hash || !hash->tabla)
		return 0;

	return hash->ocupados;
}

//Pre: -
//Post: -
void destruir_ramificacion(par_t *par, void (*destructor)(void *))
{
	if(!par)
		return;

	if (par->par_siguiente != NULL)
		destruir_ramificacion(par->par_siguiente, destructor);

	if (destructor != NULL)
		destructor(par->valor);
	free(par->clave);
	free(par);
}

//Pre: -
//Post: -
void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {

		if(hash->tabla[i] != NULL)
			destruir_ramificacion(hash->tabla[i], NULL);
	}
	free(hash->tabla);
	free(hash);
}

//Pre: -
//Post: -
void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash || !destructor)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {

		if (hash->tabla[i] != NULL)
			destruir_ramificacion(hash->tabla[i], destructor);
	}
	free(hash->tabla);
	free(hash);
}

//Pre: -
//Post: -
size_t hash_con_cada_clave(hash_t *hash, bool (*f)(const char *clave, void *valor, void *aux),void *aux)
{
	if (!hash || !f)
		return 0;

	int i = 0;
	bool seguir_iterando = true;
	size_t claves_encontradas = 0;

	par_t *par_auxiliar = NULL;

	while ( i < hash->capacidad && seguir_iterando == true) {

		if (hash->tabla[i] != NULL) {
			claves_encontradas++;
			seguir_iterando = f(hash->tabla[i]->clave, hash->tabla[i]->valor, aux);
			par_auxiliar = hash->tabla[i]->par_siguiente;

			while (par_auxiliar != NULL && seguir_iterando == true) {
				claves_encontradas++;
				seguir_iterando = f(par_auxiliar->clave, par_auxiliar->valor, aux);
				par_auxiliar = par_auxiliar->par_siguiente;
			}
		}
		i++;
	}
	return claves_encontradas;
}