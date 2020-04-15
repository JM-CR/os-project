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
