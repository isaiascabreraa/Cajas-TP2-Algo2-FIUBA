#ifndef __INVENTARIO_H__
#define __INVENTARIO_H__

typedef struct inventario_de_cajas inventario_t;

//Pre: La capacidad debe ser mayor que cero.
//Post: -
inventario_t *inventario_crear(int capacidad);

//Pre: -
//Post: Llena el inverntario con los archivos enviados por parametro y crea un indice de los mismos.
inventario_t *llenar_inventario(inventario_t *inventario, int capacidad, char **archivos);

//Pre: -
//Post: Genera una nueva caja y la agrega al inventario de cajas.
void *mezclar_cajas(inventario_t *inventario, char* nombre_uno, char *nombre_dos, char *nombre_nueva_caja);

//Pre: -
//Post: Devuelve la cantidad total de pokemones que hay en inventario.
size_t cantidad_pokemones(inventario_t *inventario);

//Pre: -
//Post: Muestra por pantalla un menu con diferentes opciones.
int abrir_menu(inventario_t *inventario, char comandos);

//Pre: -
//Post: Destruye todos los elementos del inventario, incluyendo el mismo.
void inventario_destruir(inventario_t *inventario_de_cajas);


#endif /* __INVENTARIO_H__ */


