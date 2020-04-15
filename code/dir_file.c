// Revisar dir_files.h para más información.
// File: dir_file.c
// Date: 14/04/20

// ------------------------------------------
// System and application specific headers
// ------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "dir_file.h"


// -----------------------------
// Private elements
// -----------------------------

/* Private macros and constants */

/* Private types */

/* Private global variables */

/* Private functions */

/**
 * Ordena un arreglo de strings alfabéticamente.
 *
 * @param elementos Arreglo a ordenar.
 */
static void ordenaAlfabeticamente( Dir_t *elementos, int leidos ) {
	Dir_t temp;
	for( int i = 0; i < leidos - 1; ++i ) {
		for( int j = i + 1; j < leidos; ++j ) {
			if( strcmp(elementos[i].nombre, elementos[j].nombre) > 0 ) {
				temp = elementos[i];
				elementos[i] = elementos[j];
				elementos[j] = temp;
			}
		}
	}	
}


// -----------------------------
// Public elements
// -----------------------------

/* Implementation of the public functions */

int leerDirectorio( char *nombre, Dir_t contenedor[] ) {
	// Abrir directorio
	struct dirent *dp = NULL;
	DIR *dir = opendir(nombre);

	// Leer elementos del directorio
	unsigned int i = 0;
	while ( (dp = readdir(dir)) != NULL ) {
		contenedor[i].tipo = dp->d_type;
		contenedor[i].nombre = strdup(dp->d_name);
		++i;
	}
	ordenaAlfabeticamente(contenedor, i);

	// Cerrar
	closedir(dir);
	return i;
}

bool esDirectorio( Dir_t *elemento ) {
	if ( elemento->tipo == DT_DIR ) {
		return true;
	} else {
        return false;
    }
}

bool esArchivo( Dir_t *elemento ) {
	if ( elemento->tipo == DT_REG ) {
		return true;
	} else {
        return false;
    }
}

char *rutaActual( void ) {
	char ruta[PATH_MAX];
	return getcwd(ruta, sizeof(ruta));
}

char *concatena( char *origen, char *destino ) {
	char buffer[PATH_MAX];
	strcpy(buffer, origen);
	strcat(buffer, "/");
	return 	strcat(buffer, destino);
}
