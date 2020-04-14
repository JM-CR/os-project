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

#endif
