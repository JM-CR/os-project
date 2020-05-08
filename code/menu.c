// Revisar menu.h para más información.
// File: menu.c
// Date: 14/04/20

// ------------------------------------------
// System and application specific headers
// ------------------------------------------
#include <curses.h>
#include <unistd.h>
#include "menu.h"


// -----------------------------
// Private elements
// -----------------------------

/* Private global variables */

static int cursor = 0;   // Posición del cursor

/* Private functions */

/**
 * Configura el entorno gráfico para usar ncurses.
 */
static void configurarVentana( void ) {
	initscr();   // Iniciar ventana
    cbreak();    // Permitir un caracter a la vez
	noecho();    // Suprimir caracteres introducidos
}

/**
 * Ejecuta una acción según el elemento seleccionado.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leídos.
 */
static void cargarSeleccion( Dir_t *elementos, int *leidos ) {
    // Inicializar
    Dir_t seleccion = elementos[cursor];
    char *nombre = seleccion.nombre;
    char *destino = concatena(rutaActual(), nombre);

    // Procesar
    if ( esArchivo(&seleccion) ) {
        abrirEditor(destino);
    } else if ( esDirectorio(&seleccion) ) {
        chdir(destino);
        *leidos = leerDirectorio(destino, elementos);
        cursor = 0;
    }
}

/**
 * Controla la interacción con el usuario.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leídos.
 * @return Caracter leido.
 */
static char accionDelUsuario( Dir_t *elementos, int *leidos ) {
    // Leer caracter
    char caracter = getch();

    // Procesar
    switch ( caracter ) {
    case 'A':  /* Arriba */
        cursor = (cursor > 0) ? (cursor - 1) : (*leidos - 1);
        break;
    case 'B':  /* Abajo */
        cursor = (cursor < *leidos - 1) ? (cursor + 1) : 0;
        break;
    case '\n': /* Enter */
        cargarSeleccion(elementos, leidos);
        break;
    }
    return caracter;
}

/**
 * Imprime los archivos y directorios leídos.
 *
 * @param elementos Contenedor con los elementos.
 * @param leidos Número de elementos leidos.
 */
static void listarElementos( Dir_t *elementos, size_t leidos ) {
    // Verificar límites del contenedor
    if ( cursor < 0 ) {
        cursor = leidos - 1;
    } else if ( cursor > leidos - 1 ) {
        cursor = 0;
    }
    
    // Calcular segmento activo
    int lineaInicial = (cursor / MAX_ARCHIVOS) * MAX_ARCHIVOS;
    int rango = lineaInicial + MAX_ARCHIVOS;
    int lineaFinal = rango > leidos ? leidos : rango;

    for ( unsigned int j = 0, i = lineaInicial; i < lineaFinal; ++i, ++j ) {
        // Mostrar cursor
        if ( i == cursor ) {
            attron(A_REVERSE);  /* Inicio subrayado */
        }
        // Añadir contenido
        mvprintw(4 + j, 5, (elementos + i)->nombre);
        attroff(A_REVERSE);  /* Fin subrayado */
    }

    // Posicionar cursor
    move(4 + cursor % MAX_ARCHIVOS, 4);
    refresh();
}

/**
 * Muestra el estado actual de la ventana.
 *
 * @param ruta Ruta absoluta.
 */
static void estadoActual( char *ruta ) {
    move(2, 5);
    printw("Cursor: %d. Ruta actual: %s", cursor, ruta);
}


// -----------------------------
// Public elements
// -----------------------------

/* Implementation of the public functions */

void mostrarInterfaz( void ) {
    // Leer contexto actual
	Dir_t elementos[MAX_LEIDOS];
	int leidos = leerDirectorio(rutaActual(), elementos);

    // Cargar entorno gráfico
	char caracter;
	configurarVentana();
	do {
		erase();
		estadoActual(rutaActual());
		listarElementos(elementos, leidos);
		caracter = accionDelUsuario(elementos, &leidos);
	} while ( caracter != 24 );

	// Salir
	endwin();
}
