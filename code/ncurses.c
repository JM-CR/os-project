// Revisar ncurses.h para más información.
// File: ncurses.c
// Date: 14/04/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include <unistd.h>
#include "ncurses.h"


// -----------------------------
// Private elements
// -----------------------------

/* Private functions */

/**
 * Ejecuta una acción según el elemento seleccionado.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leídos.
 * @param cursor Posición del cursor.
 */
static void cargarSeleccion( Dir_t *elementos, int *leidos, int *cursor ) {
    // Inicializar
    Dir_t seleccion = elementos[*cursor];
    char *nombre = seleccion.nombre;
    char *destino = concatena(rutaActual(), nombre);

    // Procesar
    if ( esArchivo(&seleccion) ) {
        abrirEditor(destino);
    } else if ( esDirectorio(&seleccion) ) {
        chdir(destino);
        *leidos = leerDirectorio(destino, elementos);
        *cursor = 0;
    }
}


// -----------------------------
// Public elements
// -----------------------------

/* Implementation of the public functions */

void configurarVentana( void ) {
	initscr();   // Iniciar ventana
    cbreak();    // Permitir un caracter a la vez
	noecho();    // Suprimir caracteres introducidos
}

void imprimirArchivos( Dir_t *elementos, size_t leidos, int *cursor ) {
    // Límite del contenedor
    if ( *cursor < 0 ) {
        *cursor = leidos - 1;
    } else if ( *cursor > leidos - 1 ) {
        *cursor = 0;
    }
    
    // Mostrar contenido
    int lineaInicial = (*cursor / MAX_ARCHIVOS) * MAX_ARCHIVOS;
    int rango = lineaInicial + MAX_ARCHIVOS;
    int lineaFinal = rango > leidos ? leidos : rango;
    for ( unsigned int i = lineaInicial, j = 0; i < lineaFinal; ++i, ++j ) {
        // Mostrar cursor
        if ( i == *cursor ) {
            attron(A_REVERSE);  /* Inicio subrayado */
        }

        // Añadir contenido
        mvprintw(4 + j, 5, (elementos + i)->nombre);
        attroff(A_REVERSE);  /* Fin subrayado */
    }

    // Posicionar cursor
    move(4 + *cursor % MAX_ARCHIVOS, 4);
    refresh();
}

char leerTeclado( Dir_t *elementos, int *leidos, int *cursor ) {
    // Leer caracter
    char caracter = getch();

    // Procesar
    switch ( caracter ) {
    case 'A':  /* Arriba */
        *cursor = (*cursor > 0) ? *cursor - 1 : *leidos - 1;
        break;
    case 'B':  /* Abajo */
        *cursor = (*cursor < *leidos - 1) ? *cursor + 1 : 0;
        break;
    case '\n': /* Enter */
        cargarSeleccion(elementos, leidos, cursor);
        break;
    }
    return caracter;
}

void imprimirEstado( int cursor, char *ruta ) {
    move(2, 5);
    printw("Cursor: %d. Ruta actual: %s", cursor, ruta);
}
