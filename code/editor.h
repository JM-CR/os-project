// Funcioens para controlar el editor de texto.
// Author: Josue Mosiah Contreras Rocha
// File: editor.h
// Date: 17/02/20

#ifndef EDITOR_H
#define EDITOR_H

// -----------------------------
// System headers
// -----------------------------
#include "dir_file.h"


// -----------------------------
// Public interface
// -----------------------------

/* Constants */

#define MAX_LINEAS 25     // Lineas en pantalla

/* Function prototypes */

/**
 * Abre el editor hexadecimal al detectar un archivo.
 *
 * @param ruta Ruta absoluta.
 */
void abrirEditor( char *ruta );

#endif
