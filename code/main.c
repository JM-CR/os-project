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
	char c;
	int cursor = 0;
	int leidos = leerDirectorio(rutaActual(), elementos);

	// Cargar entorno gráfico
	configurarVentana();
	do {
		erase();
		imprimirEstado(cursor, rutaActual());
		imprimirArchivos(elementos, leidos, &cursor);
		c = leerTeclado(elementos, &leidos, &cursor);
	} while ( c != 24 );

	// Salir
	endwin();
	return 0;
}
