#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define OPERACION_VALIDA true
#define OPERACION_INVALIDA false

//Pre: -
//Post: Devuelve true si el siguiente nodo al que se apunta es NULO.
bool es_el_ultimo(nodo_t *nodo)
{
	if (!nodo)
		return NULL;
	return(nodo->siguiente == NULL);
}

//Pre: -
//Post: Devuelve true si la posicion es menor a la cantidad de elementos de la lista, si no false.
bool es_posicion_valida(size_t cantidad, size_t posicion)
{
	return (posicion < cantidad);
}

/*
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t *lista_crear()
{
	lista_t *lista_crear = calloc(1, sizeof(lista_t));

	if (!lista_crear)
		return NULL;
	else
		return lista_crear;
}

/*
 * Inserta un elemento al final de la lista.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t *lista_insertar(lista_t *lista, void *elemento){

	if (!lista)
		return NULL;

	nodo_t *nodo_nuevo = malloc(sizeof(nodo_t));
	if (!nodo_nuevo)
		return NULL;

	void *elemento_enviado = elemento;

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nodo_nuevo;
		lista->nodo_fin = nodo_nuevo;

	} else {
		lista->nodo_fin->siguiente = nodo_nuevo;
		lista->nodo_fin = nodo_nuevo;
	}
	nodo_nuevo->siguiente = NULL;
	nodo_nuevo->elemento = elemento_enviado;
	(lista->cantidad)++;

	return lista;
}

/*
 * Inserta un elemento en la posicion indicada, donde 0 es insertar
 * como primer elemento y 1 es insertar luego del primer elemento.  
 * En caso de no existir la posicion indicada, lo inserta al final.
 *
 * Devuelve NULL si no pudo insertar el elemento a causa de un error, o la lista en caso de exito.
 */
lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento, size_t posicion)
{
	if (!lista)
		return NULL;

	lista_t *lista_auxiliar = NULL;

	if (lista_vacia(lista) || !es_posicion_valida(lista_tamanio(lista), posicion)){
		lista_auxiliar = lista_insertar(lista, elemento);
		return lista_auxiliar;
	}

	nodo_t *nodo_insertar = malloc(sizeof(nodo_t));
	if (!nodo_insertar)
		return NULL;
	
	nodo_insertar->elemento = elemento;

	size_t posicion_actual = 1;
	nodo_t *nodo_actual = lista->nodo_inicio;

	if (posicion == 0) {
		nodo_insertar->siguiente = nodo_actual;
		lista->nodo_inicio = nodo_insertar;
	
	} else {
		while (!es_el_ultimo(nodo_actual) && posicion_actual < posicion) {
			nodo_actual = nodo_actual->siguiente;
			posicion_actual++;
		}	
		nodo_insertar->siguiente = nodo_actual->siguiente;
		nodo_actual->siguiente = nodo_insertar;
	}
	(lista->cantidad++);
	return lista;
}

//Pre: -
//Post: -
void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	void *elemento_removido = NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;

	if (lista_tamanio(lista) == 1) {
		elemento_removido = lista->nodo_inicio->elemento;
		lista->nodo_inicio = NULL;
		lista->nodo_fin = NULL;
		free(nodo_actual);
	} else {
		while (!es_el_ultimo(nodo_actual->siguiente)) {
			nodo_actual = nodo_actual->siguiente;
		}

		nodo_actual->siguiente = NULL;
		elemento_removido = lista->nodo_fin->elemento;
		free(lista->nodo_fin);
		lista->nodo_fin = nodo_actual;
	}
	(lista->cantidad--);
	return elemento_removido;
}

/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento de la lista.
 *
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.
 *
 * Devuelve el elemento removido de la lista o NULL en caso de error.
 *
 */
void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	size_t posicion_actual = 1;
	void *elemento_removido = NULL;

	nodo_t *nodo_auxiliar = NULL;
	nodo_t *nodo_actual = lista->nodo_inicio;

	if (lista_tamanio(lista) == 1 || !es_posicion_valida( lista_tamanio(lista), posicion))
		return lista_quitar(lista);

	if (posicion == 0) {
		lista->nodo_inicio = nodo_actual->siguiente;
		elemento_removido = nodo_actual->elemento;
		free(nodo_actual);
	} else {
		while (!es_el_ultimo(nodo_actual->siguiente) && posicion_actual < posicion) {
			nodo_actual = nodo_actual->siguiente;
			posicion_actual++;
		}

		if (es_el_ultimo(nodo_actual->siguiente))
			return lista_quitar(lista);
		else {
			nodo_auxiliar = nodo_actual->siguiente;
			nodo_actual->siguiente = nodo_auxiliar->siguiente;
			elemento_removido = nodo_auxiliar->elemento;
			free(nodo_auxiliar);
		}
	}
	lista->cantidad--;
	return elemento_removido;
}

/*
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
*/ 
void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista) || !es_posicion_valida(lista_tamanio(lista), posicion))
		return NULL;

	nodo_t *nodo_actual = lista->nodo_inicio;

	if (posicion == 0)
		return nodo_actual->elemento;

	size_t posicion_actual = 0;
	while (posicion_actual < posicion) {
		nodo_actual = nodo_actual->siguiente;
		posicion_actual++;
	}
	return (nodo_actual->elemento);
}

/*
 * Devuelve el primer elemento de la lista que cumple la condición
 * comparador(elemento, contexto) == 0.
 *
 * Si no existe el elemento devuelve NULL.
*/
void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *), void *contexto)
{
	if (!lista || !comparador || lista_vacia(lista))
		return NULL;

	nodo_t *nodo_actual = lista->nodo_inicio;
	size_t posicion_actual = 0;

	while (es_posicion_valida(lista_tamanio(lista), posicion_actual)) {

		if (nodo_actual->elemento != NULL && comparador(nodo_actual->elemento, contexto) == 0)
			return nodo_actual->elemento;

		nodo_actual = nodo_actual->siguiente;
		posicion_actual++;
	}

	if (!es_posicion_valida(lista_tamanio(lista), posicion_actual))
		return NULL;
	else
		return nodo_actual->elemento;
}

/*
 * Devuelve el primer elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void *lista_primero(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;
	return lista->nodo_inicio->elemento;
}

/*
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía o no existe.
 */
void *lista_ultimo(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	return lista->nodo_fin->elemento;
}


/*
 * Devuelve true si la lista está vacía (o no existe) o false en caso contrario.
 */
bool lista_vacia(lista_t *lista)
{
	if (!lista || lista->cantidad == 0)
		return true;
	else
		return false;
}

/*
 * Devuelve la cantidad de elementos almacenados en la lista.
 * Una lista que no existe no puede tener elementos.
 */
size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;
	return lista->cantidad;
}

/*
 * Libera la memoria reservada por la lista.
*/
void lista_destruir(lista_t *lista)
{
	if (!lista)
		return;
	lista_destruir_todo(lista, NULL);
}

/*
 * Libera la memoria reservada por la lista pero además aplica la función
 * destructora dada (si no es NULL) a cada uno de los elementos presentes en la
 * lista.
 */
void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (!lista)
		return;

	nodo_t *nodo_actual = lista->nodo_inicio;
	
	while (nodo_actual != NULL) {
		nodo_t *nodo_destruir = nodo_actual;
		nodo_actual = nodo_actual->siguiente;

		if (funcion != NULL)
			funcion(nodo_destruir->elemento);

		free(nodo_destruir);
	}
	free(lista);
}

//Pre: -
//Post: -
lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (!iterador)
		return NULL;

	iterador->lista = lista;
	iterador->corriente = lista->nodo_inicio;

	return iterador;
}

//Pre: -
//Post: -
bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->lista || !iterador->corriente)
		return false;

	if (iterador->corriente != NULL)
		return true;
	else
		return false;
}

//Pre: -
//Post: -
bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || !lista_iterador_tiene_siguiente(iterador))
		return false;

	iterador->corriente = iterador->corriente->siguiente;
	return iterador->corriente;
}

/*
 * Devuelve el elemento actual del iterador o NULL en caso de que no
 * exista dicho elemento o en caso de error.
 */
void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || !iterador->corriente)
		return NULL;
	return iterador->corriente->elemento;
}

/*
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (!iterador)
		return;
	free(iterador);
}

//Pre:
//Post:
size_t lista_recorrida( nodo_t *nodo, bool (*funcion)(void *, void *), void *contexto)
{
	if (!nodo)
		return 0;

	if (funcion(nodo->elemento, contexto))
		return 1 + lista_recorrida(nodo->siguiente, funcion, contexto);
	else
		return 1;
}

/*
 * Iterador interno. Recorre la lista e invoca la funcion con cada elemento de
 * la misma como primer parámetro. Dicha función puede devolver true si se deben
 * seguir recorriendo elementos o false si se debe dejar de iterar elementos.
 *
 * El puntero contexto se pasa como segundo argumento a la función del usuario.
 *
 * La función devuelve la cantidad de elementos iterados o 0 en caso de error
 * (errores de memoria, función o lista NULL, etc).
 *
 */
size_t lista_con_cada_elemento(lista_t *lista, bool(*funcion)(void *, void *), void *contexto)
{
	if (!lista || !funcion || lista_vacia(lista) || !contexto)
		return 0;

	nodo_t *nodo_actual = lista->nodo_inicio;
	size_t cantidad_iteraciones = lista_recorrida( nodo_actual, funcion, contexto);
	
	return cantidad_iteraciones;
}

