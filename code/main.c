// File: main.c
// Date: 11/04/20

// ------------------------------------------
// System and aplication specific headers
// ------------------------------------------
#include "dir_file.h"
#include "view.h"

#define MAX_LEIDOS 128

int main(void) {
	// Cargar contexto actual
	Dir_t res[MAX_LEIDOS];
	int max = leerDirectorio(".", res), cursor = 0;
	char c;

	// Cargar entorno gráfico
	configurarVentana();
	do {
		imprimirEstado(cursor, max);
		imprimirArchivos(res, max, cursor);
		c = leerTeclado(res, &max, &cursor);
	} while ( c != 'q' );

	// Quit
	endwin();
	return 0;
}
