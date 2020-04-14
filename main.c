#include <dirent.h>
#include <curses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct s_dir {
	int type;
	char *name;
} res[128];

static int read( char *dirName ) {
	// Initialize
	DIR *dir = opendir(dirName);
	struct dirent *dp;
	int i = 0;

	// Read full dir
	while ( (dp = readdir(dir)) != NULL ) {
		res[i].type = dp->d_type;
		res[i].name = strdup(dp->d_name);
		++i;
	}

	// Close
	closedir(dir);
	return i;
}

static bool checkContext( int pos, int *max ) {
	// Guard
	if ( res[i].type == DT_REG ) {
		return false;
	}

	// Update current dir
	*max = read(res[i].name);
	return true;
}

int main(void) {
	// Initialize windows
	int c, max = read("."), i = 0;
	initscr();
	raw();
	noecho();

	// Start scanning
	do {
		// Print dirs & files
		for ( int j = 0; j < max; ++j ) {
			if ( j == i ) {
				attron(A_REVERSE);
			}
			mvprintw(5 + j, 5, res[j].name);
			attroff(A_REVERSE);
		}
		move(5 + i, 5);
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
			if ( !checkContext(i, &max) ) {
				move(3, 5);
				printw("Seleccionaste %s", res[i].name);
				exit(EXIT_SUCCESS);
			} else {
				i = 0;
				continue;
			}
			break;
		}

		// Current position
		move(2, 5);
		printw("Estoy en %d. Leí: %d", i, max);
	} while ( c != 'q' );

	// Quit
	endwin();
	return 0;
}
