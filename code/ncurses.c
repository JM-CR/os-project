// Revisar ncurses.h para más información.
// File: ncurses.c
// Date: 14/04/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "ncurses.h"


// -----------------------------
// Private elements
// -----------------------------

/* Private macros and constants */

/* Private types */

/* Private global variables */

/* Private functions */

/**
 * Lee un caracter para usar en el editor.
 * 
 * @return Caracter leido en forma de número.
 */
int leeChar( void ) {
    // Inicializar
    int chars[5];
    int ch, i = 0;

    // Espera activa
    nodelay(stdscr, TRUE);
    while((ch = getch()) == ERR );
    ungetch(ch);
    while((ch = getch()) != ERR ) {
        chars[i++] = ch;
    }

    // Convertir char a número
    int res = 0;
    for( int j = 0; j < i; j++ ) {
        res <<= 8;
        res |= chars[j];
    }
    return res;
}

/**
 * Traduce las coordenadas del cursor a index para el arreglo map.
 *
 * @param cursorX Posición del cursor en X.
 * @param cursorY Posición del cursor en Y.
 * @return Index para el archivo mapeado.
 */
static unsigned int obtenIndex( int cursorX, int cursorY ) {
    unsigned int index = 0;
    if ( cursorY < 48 ) {
        index = cursorX * 16 + cursorY / 3;
    } else {
        index = cursorX * 16 + (cursorY - 48);
    }
    return index;
}

/**
 * Imprime el contenido de un archivo.
 *
 * @param mapeo Direcciones mapeadas.
 * @param cursorX Posición del cursor en X.
 * @param cursorY Posición del cursor en Y.
 * @return Número de líneas mostradas.
 */
static int imprimeArchivo( char *mapeo, int cursorX, int cursorY ) {
    // Mostrar contenido
    int lineas = 25;
    for ( unsigned int i = 0 ; i < lineas ; ++i ) {
        char *l = hazLinea(mapeo, i * 16);
        mvprintw(i + 1, 2, l);
    }

    // Posicionar cursor
    move(cursorX + 1, cursorY + 11);
    refresh();

    return lineas;
}

/**
 * Mueve el cursor dentro del editor.
 *
 * @param cursorX Posición del cursor en X.
 * @param cursorY Posición del cursor en Y.
 * @param lineas Cantidad de líneas mostradas en pantalla.
 * @return Caracter leido.
 */
static int moverCursor( int *cursorX, int *cursorY, int lineas, char *mapeo) {
    // Leer caracter
    int caracter = leeChar();

    // Procesar
    switch ( caracter ) {
    case 0x1B5B41:  /* Arriba */
        *cursorX = (*cursorX > 0) ? *cursorX - 1 : lineas - 1;
        break;
    case 0x1B5B42:  /* Abajo */
        *cursorX = (*cursorX < lineas - 1) ? *cursorX + 1 : 0;
        break;
    case 0x1B5B43:  /* Derecha */
        if ( *cursorY < 48 ) {
            *cursorY += 3;
        } else if ( *cursorY < 63 ) {
            *cursorY += 1;
        } else if ( *cursorY == 63 ) {
            *cursorY = 0;
        }
        break;
    case 0x1B5B44:  /* Izquierda */
        if ( *cursorY == 0 ) {
            *cursorY = 63;
        } else if ( *cursorY <= 48 ) {
            *cursorY -= 3;
        } else if ( *cursorY <= 63 ) {
            *cursorY -= 1;
        }
        break;

    default: 
        if(*cursorX<16){
            char n= tolower(caracter);
            if((n>='0' && n<='9') || (n>='a' && n<= 'f')){
                mapeo[obtenIndex(*cursorX, *cursorY)] = n;
            }
        }
        else{
            if(isprint(caracter)){
                mapeo[obtenIndex(*cursorX,*cursorY)]=caracter;
                char *l=hazLinea(mapeo,*cursorY*16);
                mvprintw(*cursorY,0,l);
            }
        }
        break;

    }

    return caracter;
}

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
        // Abrir archivo
        int lineas, cursorX = 0, cursorY = 0;
        int fd = abrirArchivo(destino);
        char c, *mapeo = mapearArchivo(fd);

        // Abrir editor
        do {
            erase();
            lineas = imprimeArchivo(mapeo, cursorX, cursorY);
            c = moverCursor(&cursorX, &cursorY, lineas, mapeo);
        } while ( c != 24 );
        close(fd);

    } else if ( esDirectorio(&seleccion) ) {
        // Cambiar ruta
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

void imprimirArchivos( Dir_t *elementos, size_t leidos, int cursor ) {
    // Vaciar contenedor
    for ( unsigned int i = 0; i < leidos; ++i ) {
        // Mostrar cursor
        if ( i == cursor ) {
            attron(A_REVERSE);  /* Inicio subrayado */
        }

        // Añadir contenido
        mvprintw(5 + i, 5, (elementos + i)->nombre);
        attroff(A_REVERSE);  /* Fin subrayado */
    }

    // Posicionar cursor
    move(5 + cursor, 4);
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
    case '\n': /* Selección */
        cargarSeleccion(elementos, leidos, cursor);
        break;
    }

    return caracter;
}

void imprimirEstado( int cursor, char *ruta ) {
    move(2, 5);
    printw("Cursor: %d. Ruta actual: %s", cursor, ruta);
}
