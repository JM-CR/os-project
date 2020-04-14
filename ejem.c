#include <curses.h>

int main()
{
   char *lista[] = {"Uno", "Dos", "Tres", "Cuatro" };
   int i = 0;
   int c;
   initscr();
   raw();
   noecho(); /* No muestres el caracter leido */
//   cbreak();
   do {
      for (int j=0; j < 4; j++) {
         if (j == i) {
           attron(A_REVERSE);
         }
         mvprintw(5+j,5,lista[j]);
         attroff(A_REVERSE);
      }
      move(5+i,5);
      refresh();
      c = getch();
      switch(c) {
         case 'A':
            i = (i>0) ? i - 1 : 3;
            break;
         case 'B':
            i = (i<3) ? i + 1 : 0;
            break;
         default:
            // Nothing 
            break;
      }
      move(10,5);
      printw("Estoy en %d: Lei %d",i,c);
   } while (c != 'q');
   endwin();
   return 0;
}