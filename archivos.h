// ifndef = if not defined
// Se usa para evitar problemas entre archivos en caso de que se incluyan varias veces
#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
// Definición de la longitud máxima de una línea en el archivo CSV
#define MAX_LINE_LENGTH 1024

// Estructura para almacenar las líneas del archivo CSV
// En el fondo, es una matriz de caracteres
typedef struct {
    char lines[100][MAX_LINE_LENGTH];
    int line_count;
} CSVData;

/*
Funciones orientadas a estructuras CSV
*/

// Declaración de la función para leer un archivo CSV y almacenar las líneas
void read_csv(const char *filename, CSVData *data);

// Declaración de la función para imprimir las líneas almacenadas
void print_csv(const CSVData *data);

/*
Funciones orientadas a "string" la mayoría son implementaciones 
artesanales por restricción de librerías, en el fondo son reimplementaciones
*/

// Declaracion de la funcion maxValue
int minValue(int *a, size_t b);

// Declaración de la función para contar la cantidad de palabras en un string
void my_strcpy(char *dest, const char *src);

// Declaración de la función manual de strcat
char* my_strcat(char *dest, const char *src);

// Declaración de la función manual de strlen
int my_strlen(const char *str);

// Declaración de la función manual de strcmp
int* reverse_array(int* array, int size);

// Declaración de la función transformar string a array separado por coma
void transform_string_to_array(const char input[], int output[], int *count);
#endif // ARCHIVOS_H
