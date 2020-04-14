#include <curses.h>
#include <stdlib.h>
#include "dir_file.h"

#define MAX_LEIDOS 128

int main(void) {
	// Cargar contexto actual
	Dir_t res[MAX_LEIDOS];
	unsigned int max = leerDirectorio(".", res), i = 0, c;

	// Configurar ncurses
	initscr();
	raw();
	noecho();

	// Start scanning
	do {
		// Print dirs & files
		for ( unsigned int j = 0; j < max; ++j ) {
			if ( j == i ) {
				attron(A_REVERSE);
			}
			mvprintw(5 + j, 5, res[j].nombre);
			attroff(A_REVERSE);
		}
		move(5 + i, 4);
		refresh();
		
		// Read keyboard entry
		c = getch();
		switch ( c ) {
		case 'A':  // Arriba
			i = (i > 0) ? i - 1 : max - 1;
			break;
		case 'B':  // Abajo
			i = (i < max - 1) ? i + 1 : 0;
			break;
		case '\n': 
			if ( !esDirectorio(&res[i]) ) {
				move(3, 5);
				printw("Seleccionaste %s", res[i].nombre);
				exit(EXIT_SUCCESS);
			} else {
				i = 0;
				continue;
			}
			break;
		}

		// Current position
		move(2, 5);
		printw("Estoy en %d. Lei: %d", i, max);
	} while ( c != 'q' );

	// Quit
	endwin();
	return 0;
}
