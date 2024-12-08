#include "srep.h"
#include "archivos.h"
#include <string.h>

// Funcion artesanal de strstr() encuentra la primera ocurrencia de la subcadena, "aguja en un pajar"
// Entrada: dos cadenas de caracteres
// Salida: puntero a caracter de la primera ocurrencia de la subcadena en la objetivo
char* my_strstr(const char *objetivo, const char *busqueda) {
	// Si es nulo, retornar
    if (*busqueda == '\0') {
        return (char *)objetivo;
    }

    // Iteramos sobre la cadena objetivo
    for (; *objetivo != '\0'; objetivo++) {
        if (*objetivo == *busqueda) {
        	// Se definen variables temporales para comparar
            const char *objetivoTemp = objetivo;
            const char *reemplazoTemp = busqueda;
            // Mientras no se llegue al final de la cadena objetivo y reemplazo y sean iguales
            while (*objetivoTemp != '\0' && *reemplazoTemp != '\0' && *objetivoTemp == *reemplazoTemp) {
                objetivoTemp++;
                reemplazoTemp++;
            }
            // Si se termina, entonces retornamos objetivo
            if (*reemplazoTemp == '\0') {
                return (char *)objetivo;
            }
        }
    }

    return NULL;
}

// Parecido a strcpy, a diferencia de la implementacion en archivos, esta devuelve un puntero a caracter
// Entrada: dos cadenas de caracteres
// Salida: puntero a caracter de la cadena destino
char* my_strcpy2(char *dest, const char *src) {
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

// Implementacion artesanal de strncpy
// Entrada: dos cadenas de caracteres y un entero que representa el tamaño
// Salida: puntero a caracter de la cadena destino
char *my_strncpy(char *dest, const char *src, int n) {
    int i;
    // Copiar cada caracter de la cadena fuente a la cadena destino
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    // Añadir el fin de linea al final de la cadena destino
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Función str_replace modificada usando funciones personalizadas de cadenas
char *str_replace(char *stringOrigen, char *stringObjetivo, char *stringRemplazo) {
    // Cadena resultante
    char *resultado;  
    // Punto de inserción actual
    char *ins;     
    // Variable temporal
    char *tmp;
    // Longitud de la cadena a reemplazar
    int len_rep;   
    // Longitud de la cadena de reemplazo
    int len_with;  
    // Distancia entre el inicio y la ocurrencia de 'stringObjetivo'
    int len_front; 
    // Número de reemplazos
    int count;     

    // Caso borde, si alguna de las cadenas es nula -> retornar nulo
    if (!stringOrigen || !stringObjetivo)
        return NULL;

    len_rep = my_strlen(stringObjetivo);
    // Evita bucle infinito si objetivo es nulo
    if (len_rep == 0)
        return NULL;

    // Si stringRemplazo es NULL usa cadena vacía
    if (!stringRemplazo)
        stringRemplazo = "";
    len_with = my_strlen(stringRemplazo);

    // Cuenta el número de reemplazos necesarios
    ins = stringOrigen;
    for (count = 0; (tmp = my_strstr(ins, stringObjetivo)); ++count) {
        ins = tmp + len_rep;
    }

    // Asigna memoria para la cadena resultante, si falla memoria, retorna NULL
    tmp = resultado = malloc(my_strlen(stringOrigen) + (len_with - len_rep) * count + 1);
    if (!resultado)
        return NULL;

    // Realiza los reemplazos
    while (count--) {
        ins = my_strstr(stringOrigen, stringObjetivo);
        len_front = ins - stringOrigen;
        tmp = my_strncpy(tmp, stringOrigen, len_front) + len_front;
        tmp = my_strcpy2(tmp, stringRemplazo) + len_with;
        stringOrigen += len_front + len_rep;
    }

    // Copia el resto de la cadena original
    my_strcpy(tmp, stringOrigen);
    return resultado;
}

// Función que reemplaza un string por otro en un archivo CSV y escribe el resultado en un nuevo archivo
// Entrada: Puntero a la estructura CSVData, string objetivo, string nuevo, nombre del archivo de salida
// Salida: Archivo de texto plano con el string objetivo reemplazado por el string nuevo
/*
void srep(CSVData *data, char *string_objetivo, char *string_nuevo, char* output_file) {
    // Abrir el archivo de salida en write mode "w"
    FILE *fp = fopen(output_file, "w");
    // Manejo de error
    if (fp == NULL) {
        printf("Error al abrir el archivo de salida, inténtelo de nuevo...\n");
        return;
    }

    // Iteramos sobre lineas de la estructura archivo
    for (int i = 0; i < data->line_count; i++) {
    	// Variables auxiliares
        char* aux = data->lines[i];
        char* resultLine = str_replace(aux, string_objetivo, string_nuevo);

        // Si linea no esta vacia...
        if (resultLine != NULL) {
            // Escribir la línea modificada en el archivo
            fprintf(fp, "%s\n", resultLine);

            // Liberar la memoria asignada por str_replace
            free(resultLine);
        } else {
            // Si str_replace falla, escribir la línea original
            fprintf(fp, "%s\n", aux);
        }
    }

    // Cerrar el archivo
    fclose(fp);
}
*/

/*
void srep(CSVData *data, char *string_objetivo, char *string_nuevo, FILE *output) {
    for (int i = 0; i < data->line_count; i++) {
        char *replaced_line = str_replace(data->lines[i], string_objetivo, string_nuevo);
        fprintf(output, "%s\n", replaced_line); // Agregar salto de línea explícito
        free(replaced_line);
    }
}
*/

void srep(CSVData *data, char *string_objetivo, char *string_nuevo, FILE *output) {
    for (int i = 0; i < data->line_count; i++) {
        char *line = data->lines[i];
        size_t len = strlen(line);

        // Remove any trailing newline character
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Perform the string replacement
        char *replaced_line = str_replace(line, string_objetivo, string_nuevo);

        // Print the replaced line with a single newline
        fprintf(output, "%s\n", replaced_line);

        // Free allocated memory
        free(replaced_line);
    }
}


int main(int argc, char *argv[]){
    int opt;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;
    char* stringObjetivo = NULL;
    char* stringNuevo = NULL;

    while((opt = getopt(argc, argv, "i:o:s:S:")) != -1){
        switch(opt){
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 's':
                stringObjetivo = optarg;
                break;
            case 'S':
                stringNuevo = optarg;
                break;
            default:
                abort();
        }
    }

    if (!stringObjetivo || !stringNuevo){
        fprintf(stderr, "Error: Missing strings for replacement\n");
        return 1;
    }

    CSVData data;
    if (archivoEntrada){
        read_csv(archivoEntrada, &data);
    } else {
        read_csv_from_stream(stdin, &data);
    }

    FILE *output = stdout;
    if (archivoSalida){
        output = fopen(archivoSalida, "w");
        if (!output){
            perror("Error opening output file");
            return 1;
        }
    }

    srep(&data, stringObjetivo, stringNuevo, output);

    if (archivoSalida){
        fclose(output);
    }

    return 0;
}
