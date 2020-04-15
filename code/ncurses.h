// Manejo de la parte gráfica de ncurses.
// File: view.h
// Date: 14/04/20

// -----------------------------
// System headers
// -----------------------------
#include <curses.h>
#include "dir_file.h"


// -----------------------------
// Public interface
// -----------------------------

/* Function prototypes */

/**
 * Configura el entorno gráfico para usar ncurses.
 */
void configurarVentana( void );

/**
 * Imprime los archivos y directorios leídos.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leidos.
 * @param cursor Posición del cursor.
 */
void imprimirArchivos( Dir_t *elementos, size_t leidos, int cursor );

/**
 * Lee un caracter del teclado y lo procesa.
 * Si el usuario da enter en un directorio carga sus elementos.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leídos.
 * @param cursor Posición del cursor.
 * @return Caracter leido.
 */
char leerTeclado( Dir_t *elementos, int *leidos, int *cursor );

/**
 * Muestra el estado actual de la ventana.
 *
 * @param cursor Posición del cursor.
 * @param ruta Ruta del directorio actual.
 */
void imprimirEstado( int cursor, char *ruta );
