// Manejo de archivos y directorios.
// File: dir_file.h
// Date: 14/04/20

#ifndef DIR_FILE_H
#define DIR_FILE_H

// -----------------------------
// System headers
// -----------------------------
#include <stdbool.h>
#include <dirent.h>


// -----------------------------
// Public interface
// -----------------------------

/* Types declarations */

struct s_dir {
	int tipo;
	char *nombre;
};

typedef struct s_dir Dir_t;

/* Function prototypes */

/**
 * Carga los archivos de un directorio.
 *
 * @param nombre Directorio.
 * @param contenedor Arreglo donde se guardarán los elementos.
 * @return Número de elementos leídos.
 */
int leerDirectorio( char *nombre, Dir_t contenedor[] );

/**
 * Verifica si un elemento es un directorio.
 *
 * @param elemento Elemento a comprobar.
 * @return True si es directorio.
 */
bool esDirectorio( Dir_t *elemento );

/**
 * Verifica si un elemento es archivo.
 *
 * @param elemento Elemento a comprobar.
 * @return True si es archivo.
 */
bool esArchivo( Dir_t *elemento );

/**
 * Devuelve la ruta del directorio actual.
 *
 * @return Ruta absoluta.
 */
char *rutaActual( void );

/**
 * Hace la concatenación de dos cadenas.
 *
 * @param origen Primer cadena.
 * @param destino Segunda cadena.
 * @return Cadena unida.
 */
char *concatena( char *origen, char *destino );

/**
 * Muestra la dirección, contenido hexadecimal y caracteres de una
 * línea de texto de 16 elementos.
 *
 * @param base Inicio del archivo mapeado a memoria.
 * @param dir Offset hexadecimal.
 * @return Línea formateada.
 */
char *hazLinea( char *base, int dir );

/**
 * Abre un archivo en modo lectura.
 *
 * @param ruta Ruta del archivo.
 * @return File descriptor.
 */
int abrirArchivo( char *ruta );

/**
 * Calcula el tamaño de un archivo.
 *
 * @param fd File descriptor.
 * @return File size.
 */
int tamanoArchivo( int fd );

/**
 * Realiza un mapeo a las direcciones de un objeto en memoria.
 *
 * @param fd File descriptor.
 * @return Inicio del mapeo.
 */
char *mapearArchivo( int fd );

/**
 * Obtiene el total de líneas a imprimir de un archivo.
 *
 * @param fd File descriptor.
 * @return Número de líneas.
 */
int totalDeLineas( int fd );

#endif
