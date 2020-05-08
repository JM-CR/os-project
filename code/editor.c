// See editor.h for more info
// Author: Josue Mosiah Contreras Rocha
// File: editor.c
// Date: 17/02/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include <ctype.h>
#include <unistd.h>
#include <curses.h>
#include "editor.h"


// -----------------------------
// Private elements
// -----------------------------

/* Private global variables */

static int cursorX;       // Posición del cursor en X.
static int cursorY;       // Posición del cursor en Y.
static int lineaActiva;   // Línea de lectura activa.

/* Private functions */

/**
 * Lee un caracter para usar en el editor.
 * 
 * @return Caracter leido en forma de número.
 */
static int leeChar( void ) {
    // Espera activa | Sin bloqueo
    int i = 0, ch, chars[5];
    nodelay(stdscr, TRUE);
    while((ch = getch()) == ERR ); 
    ungetch(ch);
    while((ch = getch()) != ERR ) chars[i++] = ch;

    // Convertir char a número
    int res = 0;
    for( int j = 0; j < i; ++j ) {
        res <<= 8;
        res |= chars[j];
    }
    return res;
}

/**
 * Traduce las coordenadas del cursor a index para el arreglo map.
 *
 * @return Index para el archivo mapeado.
 */
static unsigned int obtenIndex( void ) {
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
 * @param total Total de líneas a imprimir.
 */
static void imprimeContenido( char *mapeo, int total ) {
    // Validar límite de archivo
    if ( lineaActiva < 0 ) {
        lineaActiva = total - 1;
        cursorX = total % MAX_LINEAS - 1;
    } else if ( lineaActiva > total - 1 ) {
        lineaActiva = 0;
        cursorX = 0;
    }
    
    // Calcular segmento activo
    int lineaInicial = (lineaActiva / MAX_LINEAS) * MAX_LINEAS;
    int rango = lineaInicial + MAX_LINEAS;
    int lineaFinal = rango > total ? total : rango;

    // Imprimir
    for ( unsigned int i = lineaInicial, j = 0; i < lineaFinal; ++i, ++j ) {
        char *l = hazLinea(mapeo, i * 16);
        mvprintw(j + 1, 2, l);
    }

    // Posicionar cursor
    move(cursorX + 1, cursorY + 11);
    refresh();
}

 /**
 * Mueve el cursor dentro del editor.
 *
 * @param mapeo Archivo mapeado en memoria.
 * @return Caracter leido.
 */
static int moverCursor( char *mapeo ) {
    // Leer caracter
    int caracter = leeChar();

    // Procesar
    switch ( caracter ) {
    case 0x1B5B41:  /* Arriba */
        cursorX = (cursorX > 0) ? cursorX - 1 : MAX_LINEAS - 1;
        (lineaActiva)--;
        break;
    case 0x1B5B42:  /* Abajo */
        cursorX = (cursorX < MAX_LINEAS - 1) ? cursorX + 1 : 0;
        (lineaActiva)++;
        break;
    case 0x1B5B43:  /* Derecha */
        if ( cursorY < 48 ) {
            cursorY += 3;
        } else if ( cursorY < 63 ) {
            cursorY += 1;
        } else if ( cursorY == 63 ) {
            cursorY = 0;
        }
        break;
    case 0x1B5B44:  /* Izquierda */
        if ( cursorY == 0 ) {
            cursorY = 63;
        } else if ( cursorY <= 48 ) {
            cursorY -= 3;
        } else if ( cursorY <= 63 ) {
            cursorY -= 1;
        }
        break;

    default: 
        if ( cursorX < 16 ) {
            char n = tolower(caracter);
            if((n>='0' && n<='9') || (n>='a' && n<= 'f')){
                mapeo[obtenIndex()] = n;
            }
        }
        else{
            if(isprint(caracter)){
                mapeo[obtenIndex()] = caracter;
            }
        }
        break;

    }

    return caracter;
}


// -----------------------------
// Public elements
// -----------------------------

/* Implementation of the public functions */

void abrirEditor( char *ruta ) {
    // Abrir archivo
    int fd = abrirArchivo(ruta);
    int totalLineas = totalDeLineas(fd);
    char *mapeo = mapearArchivo(fd);

    // Abrir editor
    char c;
    lineaActiva = cursorX = cursorY = 0;
    do {
        erase();
        imprimeContenido(mapeo, totalLineas);
        c = moverCursor(mapeo);
    } while ( c != 24 );
    close(fd);
}
