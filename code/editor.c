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
    int ch;
    nodelay(stdscr, TRUE);
    while((ch = getch()) == ERR );
    
    // Procesar caracter
    int chars[5], i = 0;
    ungetch(ch);
    while((ch = getch()) != ERR ) { 
        chars[i++] = ch;
    }

    // Convertir char a número
    int res = 0;
    for( int j = 0; j < i; ++j ) {
        res <<= 8;
        res |= chars[j];
    }
    return res;
}

/**
 * Traduce las coordenadas del cursor.
 *
 * @return Posición de inserción.
 */
static unsigned int indiceInsercion( void ) {
    unsigned int index = 0;
    if ( cursorY < 48 ) {
        index = lineaActiva * 16 + cursorY / 3;
    } else {
        index = lineaActiva * 16 + (cursorY - 48);
    }
    return index;
}

/**
 * Imprime el contenido de un archivo.
 *
 * @param mapeo Direcciones mapeadas.
 * @param lineasLeidas Total de líneas a imprimir.
 */
static void muestraArchivo( char *mapeo, int lineasLeidas ) {
    // Validar límites del archivo
    if ( lineaActiva < 0 ) {
        lineaActiva = lineasLeidas - 1;
        cursorX = (lineasLeidas % MAX_LINEAS) - 1;
    } else if ( lineaActiva > lineasLeidas - 1 ) {
        lineaActiva = cursorX = 0;
    }
    
    // Calcular segmento activo
    int lineaInicial = (lineaActiva / MAX_LINEAS) * MAX_LINEAS;
    int rango = lineaInicial + MAX_LINEAS;
    int lineaFinal = rango > lineasLeidas ? lineasLeidas : rango;

    for ( unsigned int j = 0, i = lineaInicial; i < lineaFinal; ++i, ++j ) {
        char *l = hazLinea(mapeo, i * 16);
        mvprintw(j + 1, 2, l);
    }

    // Posicionar cursor
    move(cursorX + 1, cursorY + 11);
    refresh();
}

/**
 * Mueve el cursor a la derecha.
 */
static void moverDerecha( void ) {
    if ( cursorY < 48 ) {
        cursorY += 3;
    } else if ( cursorY < 63 ) {
        cursorY += 1;
    } else if ( cursorY == 63 ) {
        cursorY = 0;
    }
}

/**
 * Mueve el cursor a la izquierda.
 */
static void moverIzquierda( void ) {
    if ( cursorY == 0 ) {
        cursorY = 63;
    } else if ( cursorY <= 48 ) {
        cursorY -= 3;
    } else if ( cursorY <= 63 ) {
        cursorY -= 1;
    }
}

/**
 * Convierte dos caracteres a un número hexadecimal.
 *
 * @param MSB Caracter más significativo.
 * @param LSB Caracter menos significativo.
 * @return Número hexadecimal.
 */
unsigned char hex2bin ( unsigned char MSB, unsigned char LSB ) {  
    if (MSB > '9') MSB -= 7;
    if (LSB > '9') LSB -= 7;
    return (MSB <<4) | (LSB & 0x0F);
}  

/**
 * Procesa un caracter para editar el archivo.
 *
 * @param caracter Caracter introducido.
 * @param mapeo Archivo mapeado en memoria.
 */
static void editaArchivo( int caracter, char *mapeo ) {
    if ( cursorY < 48 ) {
        char c1 = tolower(caracter);
        char c2 = tolower(leeChar());
        if ( isxdigit(c1) && isxdigit(c2) ) {
            mapeo[indiceInsercion()] = hex2bin(c1, c2);
        }
    } else {
        if ( isprint(caracter) ) {
            mapeo[indiceInsercion()] = caracter;
        }
    }
}

 /**
 * Controla la interacción del usuario con el editor.
 *
 * @param mapeo Archivo mapeado en memoria.
 * @return Caracter leido.
 */
static int accionDelUsuario( char *mapeo ) {
    // Leer caracter
    int caracter = leeChar();

    // Procesar
    switch ( caracter ) {
    case 0x1B5B41:  /* Arriba */
        cursorX = (cursorX > 0) ? (cursorX - 1) : (MAX_LINEAS - 1);
        lineaActiva--;
        break;
    case 0x1B5B42:  /* Abajo */
        cursorX = (cursorX < MAX_LINEAS - 1) ? (cursorX + 1) : 0;
        lineaActiva++;
        break;
    case 0x1B5B43:
        moverDerecha();
        break;
    case 0x1B5B44:
        moverIzquierda();
        break;
    default: 
        editaArchivo(caracter, mapeo);
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
    int lineasLeidas = totalDeLineas(fd);
    char *mapeo = mapearArchivo(fd);

    // Abrir editor
    int caracter;
    lineaActiva = cursorX = cursorY = 0;
    do {
        erase();
        muestraArchivo(mapeo, lineasLeidas);
        caracter = accionDelUsuario(mapeo);
    } while ( caracter != 24 );
    close(fd);
}
