#include "abb.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//Pre: El nodo debe ser valido.
//Post: -
bool puedo_ir_a_la_izquierda(nodo_abb_t *nodo)
{
	return nodo->izquierda != NULL;
}

//Pre: El nodo debe ser valido.
//Post: -
bool puedo_ir_a_la_derecha(nodo_abb_t *nodo)
{
	return nodo->derecha != NULL;
}

//Pre: -
//Post: -
abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador)
		return NULL;

	abb_t *abb_creado = calloc(1, sizeof(abb_t));
	if (!abb_creado)
		return NULL;

	abb_creado->comparador = comparador;
	return abb_creado;
}

//Pre: -
//Post: -
nodo_abb_t *nodo_crear(nodo_abb_t *nodo_actual, void* elemento)
{
	nodo_abb_t *nodo_nuevo = calloc(1, sizeof(nodo_abb_t));
	if (!nodo_nuevo)
		return NULL;
	
	nodo_nuevo->elemento = elemento;
	return nodo_nuevo;
}

//Pre: Anteriormente se debe verificar la existencia del comparador y el arbol.
//Post: -
nodo_abb_t *insertar_rec(abb_t* arbol, nodo_abb_t* nodo_actual, void *elemento)
{
	if (!nodo_actual){
		nodo_abb_t *nodo_creado = nodo_crear (nodo_actual, elemento);
		
		if(nodo_creado != NULL)
			arbol->tamanio++;
		return nodo_creado; 
	}

	int comparador = arbol->comparador(elemento, nodo_actual->elemento);

	if (comparador > 0)
		nodo_actual->derecha =  insertar_rec (arbol, nodo_actual->derecha, elemento);

	else if (comparador <= 0)
		nodo_actual->izquierda =  insertar_rec (arbol, nodo_actual->izquierda, elemento);

	return nodo_actual;
}

//Pre: -
//Post: Inserta un elemento segun su clave (los mas chicos la izquierda y los mas grandes a la derecha).
abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;

	arbol->nodo_raiz = insertar_rec(arbol, arbol->nodo_raiz, elemento);

	return arbol;
}

//Pre: -
//Post: Asigna el predecesor inmediato del nodo que se desea retirar.
nodo_abb_t *asignar_nodo_predecesor(nodo_abb_t *nodo_actual, nodo_abb_t** nodo_predecedor)
{
	if (!nodo_actual || !puedo_ir_a_la_derecha(nodo_actual)) {
		*nodo_predecedor = nodo_actual;
		return nodo_actual->izquierda;
	}
	nodo_actual->derecha = asignar_nodo_predecesor(nodo_actual->derecha, nodo_predecedor);
	return nodo_actual;
}

//Pre: Anteriormente se debe verificar la existencia del comparador y el arbol.
//Post: -
nodo_abb_t *quitar_rec(abb_t *arbol, nodo_abb_t *nodo_actual, void *elemento, void** elemento_eliminado)
{
	if (!nodo_actual)
		return NULL;

	nodo_abb_t *nodo_auxiliar = NULL;
	int comparador = arbol->comparador(elemento, nodo_actual->elemento);

	if (comparador > 0)
		nodo_actual->derecha = quitar_rec(arbol, nodo_actual->derecha, elemento, elemento_eliminado);
	else if (comparador < 0)
		nodo_actual->izquierda = quitar_rec(arbol, nodo_actual->izquierda, elemento, elemento_eliminado);
	else {
		*elemento_eliminado = nodo_actual->elemento;
		arbol->tamanio--;

		if (nodo_actual->derecha == NULL && nodo_actual->izquierda == NULL) {
			free(nodo_actual);
			return NULL;

		} else if (nodo_actual->derecha == NULL || nodo_actual->izquierda == NULL) {
			nodo_auxiliar = nodo_actual->derecha;
			if(nodo_auxiliar == NULL)
				nodo_auxiliar = nodo_actual->izquierda;
			free(nodo_actual);
			return nodo_auxiliar;
		} else {
			nodo_abb_t* nodo_predecedor = NULL;
			nodo_actual->izquierda = asignar_nodo_predecesor(nodo_actual->izquierda, &nodo_predecedor);
			nodo_predecedor->izquierda = nodo_actual->izquierda;
			nodo_predecedor->derecha = nodo_actual->derecha;
			free(nodo_actual);
			return nodo_predecedor;
		}
	}
	return nodo_actual;
}

//Pre: -
//Post: -
void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol || !arbol->comparador || !arbol->nodo_raiz)
		return NULL;

	void *elemento_eliminado = NULL;
	arbol->nodo_raiz = quitar_rec (arbol, arbol->nodo_raiz, elemento, &elemento_eliminado);

	return elemento_eliminado;
}

//Pre: -
//Post: -
bool abb_vacio(abb_t *arbol)
{
	if (!arbol || !arbol->nodo_raiz || abb_tamanio(arbol) == 0)
		return true;
	return false;
}

//Pre: Comparador debe ser capaz de trabajar con elementos NULL.
//Post: Devuelve el elemento buscado dentro del arbol.
void *buscar_rec( abb_t *arbol, nodo_abb_t *nodo_actual, void *elemento)
{
	if (!nodo_actual || !arbol->comparador)
		return NULL;
	
	int comparador = arbol->comparador(elemento, nodo_actual->elemento);

	if (comparador < 0)
		return buscar_rec(arbol, nodo_actual->izquierda, elemento);
	
	else if (comparador > 0)
		return buscar_rec (arbol, nodo_actual->derecha, elemento);
	else
		return nodo_actual->elemento;
}

//Pre: -
//Post: -
void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	return buscar_rec(arbol, arbol->nodo_raiz, elemento);
}

//Pre: -
//Post: -
size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;
	return arbol->tamanio;
}

//Pre: -
//Post: Segun uno de los recorridos asignados, va aplicando una funcion al elemento actual hasta que esta devuelve false.
bool recorrer_abb_cada_elemento_rec(nodo_abb_t *nodo_actual, bool (*funcion)(void *, void *), void *aux, size_t *invoncaciones_a_funcion, abb_recorrido recorrido)
{
	if (!nodo_actual)
		return true;

	bool seguir_recorriendo = true;

	if (recorrido == PREORDEN && seguir_recorriendo == true) {
		(*invoncaciones_a_funcion)++;
		if (funcion(nodo_actual->elemento, aux) == false)
			return false;
	}

	if (puedo_ir_a_la_izquierda(nodo_actual) && seguir_recorriendo == true)
		seguir_recorriendo = recorrer_abb_cada_elemento_rec(nodo_actual->izquierda, funcion, aux, invoncaciones_a_funcion, recorrido);

	if (recorrido == INORDEN && seguir_recorriendo == true) {
		(*invoncaciones_a_funcion)++;
		if (funcion(nodo_actual->elemento, aux) == false)
			return false;
	}
		
	if (puedo_ir_a_la_derecha(nodo_actual) && seguir_recorriendo == true)
		seguir_recorriendo = recorrer_abb_cada_elemento_rec(nodo_actual->derecha, funcion, aux, invoncaciones_a_funcion, recorrido);

	if (recorrido == POSTORDEN && seguir_recorriendo == true) {
		(*invoncaciones_a_funcion)++;
		if (funcion(nodo_actual->elemento, aux) == false)
			return false;
	}
	return seguir_recorriendo;
}

//Pre: -
//Post: -
size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion || abb_tamanio(arbol) == 0)
		return 0;

	size_t invoncaciones_a_funcion = 0;

	recorrer_abb_cada_elemento_rec(arbol->nodo_raiz, funcion, aux, &invoncaciones_a_funcion, recorrido);

	return invoncaciones_a_funcion;
}

//Pre: -
//Post: Segun uno de los recorridos asignados, va asignando los elementos correspondientes al vector enviado.
void recorrer_rec(abb_t *arbol, nodo_abb_t *nodo_actual, void **array, size_t tamanio_array, size_t *elementos_recorridos, abb_recorrido recorrido)
{
	if (nodo_actual == NULL)
		return;

	if (recorrido == PREORDEN && *elementos_recorridos < tamanio_array) {
		array[*elementos_recorridos] = nodo_actual->elemento;
		(*elementos_recorridos)++;
	}

	if (puedo_ir_a_la_izquierda(nodo_actual))
		recorrer_rec(arbol, nodo_actual->izquierda, array, tamanio_array, elementos_recorridos, recorrido);

	if (recorrido == INORDEN && *elementos_recorridos < tamanio_array) {
		array[*elementos_recorridos] = nodo_actual->elemento;
		(*elementos_recorridos)++;
	}

	if (puedo_ir_a_la_derecha(nodo_actual))
		recorrer_rec(arbol, nodo_actual->derecha, array, tamanio_array, elementos_recorridos, recorrido);

	if (recorrido == POSTORDEN && *elementos_recorridos < tamanio_array) {
		array[*elementos_recorridos] = nodo_actual->elemento;
		(*elementos_recorridos)++;
	}
}

//Pre: -
//Post: -
size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array, size_t tamanio_array)
{
	if (!arbol || !array || tamanio_array == 0)
		return 0;

	size_t elementos_almacenados = 0;

	recorrer_rec(arbol, arbol->nodo_raiz, array, tamanio_array, &elementos_almacenados, recorrido);

	return elementos_almacenados;
}

//Pre: -
//Post: -
void destruir_rec (nodo_abb_t *nodo_actual, void (*destructor)(void *))
{
	if (!nodo_actual)
		return;

	if (nodo_actual->izquierda != NULL)
		destruir_rec( nodo_actual->izquierda, destructor);

	if (nodo_actual->derecha != NULL)
		destruir_rec( nodo_actual->derecha, destructor);

	if (destructor != NULL)
		destructor(nodo_actual->elemento);

	free(nodo_actual);
}

//Pre: -
//Post: -
void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;
	destruir_rec(arbol->nodo_raiz, NULL);
	free(arbol);
}

//Pre: -
//Post: -
void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;
	
	destruir_rec(arbol->nodo_raiz, destructor);
	free(arbol);
}