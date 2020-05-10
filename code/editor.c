// See editor.h for more info
// Author: Josue Mosiah Contreras Rocha
// File: editor.c
// Date: 17/02/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <sys/mman.h>
#include "editor.h"

// -----------------------------
// Private elements
// -----------------------------

/* Private global variables */

static int cursorX;       // Posición del cursor en X.
static int cursorY;       // Posición del cursor en Y.
static int lineaActiva;   // Línea de lectura activa.
static char msg[50];      // Mensaje global de estado

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
        move(j + 1, 2);
        addstr(l);
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
 * Imprime un mensaje de estado en la ventana.
 */
static void imprimeEstado( void ) {
    move(27, 5); addstr(msg);
    mvprintw(
        29, 5, "Fila: %d. Columna: %d. Entrada %s.", 
        cursorX, 
        (cursorY < 48) ? (cursorY / 3 + 1) : (cursorY - 47),
        (cursorY < 48) ? "hexadecimal" : "alfanumerica"
    );
}

/**
 * Convierte dos caracteres a un número hexadecimal.
 *
 * @param MSB Caracter más significativo.
 * @param LSB Caracter menos significativo.
 * @return Número hexadecimal.
 */
unsigned char char2hex ( unsigned char MSB, unsigned char LSB ) {  
    if ( MSB > '9' ) MSB -= 7;
    if ( LSB > '9' ) LSB -= 7;
    return (MSB << 4) | (LSB & 0x0F);
}  

/**
 * Procesa un caracter para editar el archivo.
 *
 * @param caracter Caracter introducido.
 * @param mapeo Archivo mapeado en memoria.
 */
static void editaArchivo( int caracter, char *mapeo ) {
    if ( cursorY < 48 ) {
        // Leer primer caracter
        char c1 = tolower(caracter);
        mvprintw(27, 5, "Valor detectado: 0x%c.\n", c1);

        // Leer segunda caracter
        char c2 = tolower(leeChar());

        // Validar
        if ( isxdigit(c1) && isxdigit(c2) ) {
            mapeo[indiceInsercion()] = char2hex(c1, c2);
            sprintf(msg, "Ultimo valor insertado: 0x%c%c.\n", c1, c2);
        } else {
            sprintf(msg, "Combinacion invalida.\n");
        }
    } else {
        if ( caracter < 256 && isprint(caracter) ) {
            mapeo[indiceInsercion()] = caracter;
            sprintf(msg, "Ultimo valor insertado: %c\n", caracter);
        } else {
            sprintf(msg, "Caracter invalido.\n");
        }
    }
}

static void borrarCaracter(char *mapeo){
    int len = strlen(mapeo);
    int c= indiceInsercion(); 
    if ( cursorY < 48 ) {
        for(int i=0;i<len;i++){
            mapeo[c+i]=mapeo[c+1+i];
            mapeo[c+i+1]=mapeo[c+2+i];
        }
        if(cursorY>1){
            moverIzquierda();
        } 
    } else {
        for(int i=0;i<len;i++){
            mapeo[c+i]=mapeo[c+1+i];
        }
        if(cursorY>48){
            moverIzquierda();
        }
    }
    mapeo[len-1]=' '; 
}

void insertarChar(char *mapeo){
    int len = strlen(mapeo);
    int c= indiceInsercion(); 
    for(int i=0;i<len;i++){
        int temp= mapeo[c+1+i];
        mapeo[c+i+1]=mapeo[c+i];
        mapeo[c+i+2]=temp;
    }
    insch(getch());

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
    case 0x18:  /* Salir */
        break;
    case 0x7F:
        borrarCaracter(mapeo);
        break;
    case 0x1B5B327E:
        insertarChar(mapeo);
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

void abrirEditor( char *ruta) {
    // Abrir archivo
    int fdl = abrirArchivo(ruta, O_RDONLY);
    int fde = abrirArchivo(ruta, O_RDWR | O_CREAT);
    char *mapeoR = mapearArchivo(fdl, 0);
    char *mapeoRW = mapearArchivo(fde, 1);
    memcpy(mapeoRW, mapeoR, tamanoArchivo(fdl));

    // Abrir editor
    int caracter;
    int lineasLeidas = totalDeLineas(fdl);
    sprintf(msg, " ");
    lineaActiva = cursorX = cursorY = 0;
    do {
        erase();
        imprimeEstado();
        muestraArchivo(mapeoRW, lineasLeidas);
        caracter = accionDelUsuario(mapeoRW);
    } while ( caracter != 0x18 );

    // Cerrar archivo
    if ( munmap(mapeoR, tamanoArchivo(fdl)) == -1 ) {
        perror("Error un-mmapping the file");
    }
    if ( munmap(mapeoRW, tamanoArchivo(fde)) == -1 ) {
        perror("Error un-mmapping the file");
    }
    close(fdl);
    close(fde);
}
