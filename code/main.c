// File: main.c
// Date: 11/04/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include "dir_file.h"
#include "ncurses.h"

#define MAX_LEIDOS 128

int main(void) {
	// Cargar contexto actual
	Dir_t elementos[MAX_LEIDOS];
	int leidos = leerDirectorio(".", elementos), cursor = 0;
	char c;

	// Cargar entorno gráfico
	configurarVentana();
	do {
		erase();
		imprimirEstado(cursor, leidos);
		imprimirArchivos(elementos, leidos, cursor);
		c = leerTeclado(elementos, &leidos, &cursor);
	} while ( c != 'q' );

	// Quit
	endwin();
	return 0;
}
