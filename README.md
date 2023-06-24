# TP2-Cajas-Algo2-FIUBA

TP - CAJAS DE ALMACENAMIENTO POKEMON

OBJETIVO
El objetivo de este trabajo es que puedas demostrar todo lo aprendido durante el cuatrimestre (no solo respecto del uso de TDAs, si no también de diseño y estilo de código, pruebas, complejidades, etc).

SISTEMA DE GESTIÓN DE CAJAS
El sistema de gestión de cajas no es mas que un programa que cumple con los siguientes requisitos:

Recibe por parámetro (argc/argv) un listado de nombres de archivo. Estos nombres de archivo serán utilizados como los nombres de caja.

Al iniciar el programa, se intentan cargar todas las cajas pasadas por parámetro. Si alguna falla, el programa debe mostrar por pantalla un mensaje de error indicando cuáles cajas no se pueden cargar. Si no se puede cargar ninguna caja, el programa finaliza con un mensaje de error.

Al cargarse de forma exitosa por lo menos una caja, se presenta un menú en forma de texto que permite que el usuario interactúe con el programa. Y se espera a que el usuario seleccione una opción.

Cada opción del menú está identificado por una letra. Para seleccionar una opción, el usuario debe escribir esa letra y presionar enter. Si la opción es incorrecta se muestra un mensaje de error y se vuelve al menú principal explicando el motivo de error (por ejemplo: "No se puede usar ese nombre", "Esa caja no existe", "El nombre del pokemon no puede ser vacío", etc).

Cuando el usuario selecciona una opción, el programa debe ejecutar la acción correspondiente con la misma y mostrar el resultado. Luego de finalizada la operación, se debe volver a mostrar el menú principal. No debe haber interacciones de por medio (presionar una tecla para continuar o similar).

Cada comando puede requerir uno o mas parámetros. Cuando así sea, el programa debe pedir las opciones una a una (esperando a que el usuario ingrese el dato requerido).

Los comandos a implementar y sus correspondientes letras son:

(I) Mostrar inventario: Muestra un listado con los nombres de cajas disponibles y la cantidad de pokemon de cada caja.
(C) Cargar otra caja: Pide un nombre de archivo e intenta cargar una caja. Ojo que si el archivo ya fue cargado una vez, no se permite cargarlo nuevamente (no puede haber dos cajas con el mismo nombre).
(M) Combinar cajas: Pide dos nombres de cajas y un nombre de archivo. Combina las cajas y las guarda a un archivo. La nueva caja generada también queda cargada en el sistema. Tener en cuenta que el nombre de la nueva caja no puede ser repetido.
(D) Mostrar caja: Pide un nombre de caja y muestra el contenido de la caja por pantalla (muestra los datos de cada pokemon de la caja).
(B) Buscar caja: Pide un nombre de pokemon y muestra la cantidad de cajas que contienen dicho pokemon y la lista con los nombres de las cajas.
(Q) Salir: Sale del programa (obviamente se libera toda la memoria correctamente ¬_¬)
En cualquiera de los casos, cuando se produzca un error, siempre se recomienda explicarle al usuario por qué se produjo el error y cómo debe proceder para poder utilizar el programa exitosamente.

CONSIDERACIONES EXTRA
El programa/menú y su uso debe ser entendible por alguien que no leyó el enunciado.
Las letras de los comandos no diferencian entre mayúsculas y minúsculas. Ingresar 'q' o 'Q' debería ser lo mismo.
La lista de comandos propuestas son los obligatorios. Es posible (aunque para nada obligatorio) agregar mas comandos (como por ejemplo un comando de ayuda o alguna otra operación que pueda resultar interesante).
No se pueden agregar interacciones extra con el usuario donde no son necesarias (sé que a mas de uno le gusta meter algún system("clear") o similar, o un "Presione enter para continuar", pero no).
Fuera de lo pedido, se puede ser tan creativo como se quiera (formato del menu, forma de mostrar los pokemon, colores, cosas y opciones extra).
Se busca que las operaciones sean los mas eficientes posibles respecto del tiempo de ejecución (a veces puede ser necesario almacenar cosas extra para facilitar las búsquedas).
ENTREGA
La entrega deberá contar con todos los archivos necesarios para compilar y correr correctamente el programa. Dichos archivos deberán formar parte de un único archivo .zip el cual será entregado a través de la plataforma de corrección automática Chanutron't.

El archivo comprimido deberá contar, además de los archivos del TP2, con un informe que contenga la siguiente información:

Explicación de los cambios implementados al TP1. Justificar los cambios (explicar por qué se decidió el TDA utilizado).
Explicación de la implementación de cada comando pedido. Explicar cómo funcionan (y cómo utilizan el TDA Caja implementado en el TP1).
Diagramas que sirvan para entender cómo funciona tu programa (por favor, los diagramas se intercalan con la explicación, no se ponen al fondo de todo desconectado de todo el resto).
