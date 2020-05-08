// Permite controlar el menu de archivos.
// File: menu.h
// Date: 14/04/20

#ifndef MENU_H
#define MENU_H

// -----------------------------
// System headers
// -----------------------------
#include "editor.h"


// -----------------------------
// Public interface
// -----------------------------

/* Constants */

#define MAX_LEIDOS 128    // Máximo de elementos leídos
#define MAX_ARCHIVOS 23   // Archivos listados en pantalla

/* Function prototypes */

/**
 * Abre la interfaz para seleccionar un archivo.
 */
 void mostrarInterfaz( void );

#endif
