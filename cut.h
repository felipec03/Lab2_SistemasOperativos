#ifndef CUT_H
#define CUT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "archivos.h"


// Estructura para almacenar las columnas de cada línea
typedef struct {
	// Arreglo de arreglos de cadenas de caracteres (columnas por línea)
    char ***columns;
    // Número de columnas por cada línea
    int *column_count;
    // Número de líneas
    int line_count;
} CSVColumns;

// Declaración de la función split que separará las líneas según el delimitador
void split(CSVData *data, const char delimiter, CSVColumns *columns_data);

// Declaración de la función cut que cortará las columnas especificadas
char ***cut(CSVColumns *columns_data, int *cobj, int num_cobj);

// Declaración de la función que escribirá la salida en un archivo
void out(char ***cut_data, int num_lines, int num_cols, const char *filename, char delimiter);

#endif
