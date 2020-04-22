// Revisar dir_files.h para más información.
// File: dir_file.c
// Date: 14/04/20

// ------------------------------------------
// System and application specific headers
// ------------------------------------------
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

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
	// Añadir origen
	char buffer[PATH_MAX];
	strcpy(buffer, origen);

	// Verificar si es root
	if ( strcmp(origen, "/") != 0 ) {
		strcat(buffer, "/");
	}

	// Añadir destino
	return 	strcat(buffer, destino);
}

char *hazLinea( char *base, int dir ) {
    // Inicializar
    char linea[100];  /* Tamaño máximo */
    int o = 0;        /* Offset */
    
    // Dirección
    o += sprintf(linea, "%08x ", dir);

    // Contenido hexadecimal
    unsigned char a, b, c, d;
    for ( int i = 0; i < 4; i++ ) {
        a = base[dir+4 * i+0];
        b = base[dir+4 * i+1];
        c = base[dir+4 * i+2];
        d = base[dir+4 * i+3];
        o += sprintf(&linea[o], "%02x %02x %02x %02x ", a, b, c, d);
    }

    // Caracteres
    for( int i = 0; i < 16; i++ ) {
        if (isprint(base[dir+i])) {
            o += sprintf(&linea[o], "%c", base[dir + i]);
        } else {
            o += sprintf(&linea[o], ".");
        }
    }
    sprintf(&linea[o], "\n");

    // Crear copia del texto
    return strdup(linea);
}

int abrirArchivo( char *ruta ) {
    int fd;
    if ( (fd = open(ruta, O_RDWR)) == -1) {
        perror("Error abriendo el archivo");
        exit(EXIT_FAILURE);
    }
	return fd;
}

int tamanoArchivo( int fd ) {
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}

char *mapearArchivo( int fd ) {
    int fs = tamanoArchivo(fd);
    char *mapeo = mmap(0, fs, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ( mapeo == MAP_FAILED ) {
        close(fd);
        perror("Error mapeando el archivo");
        exit(EXIT_FAILURE);
    }
    return mapeo;
}
