#include "archivos.h"

// Funciones de string artesanal

int minValue(int myArray[], size_t size) {
    /* enforce the contract */
    assert(myArray && size);
    size_t i;
    int maxValue = myArray[0];

    for (i = 1; i < size; ++i) {
        if ( myArray[i] < maxValue ) {
            maxValue = myArray[i];
        }
    }
    return maxValue;
}

// String copy casero
// Entrada: dos cadenas de caracteres
// Salida: void, hace un proceso.
void my_strcpy(char *dest, const char *src) {
	// Copiar cada caracter de la cadena fuente a la cadena destino
    while (*src != '\0') {
        *dest++ = *src++;
    }
    // Añadir el terminador nulo al final de la cadena destino
    *dest = '\0';
}

// strcspn artesanal, considera saltos de linea
// Hasta encotnrar el /n almacena en la cadena destino
// Entrada: dos cadenas de caracteres
// Salida: tamaño de la cadena cómo estructura size_t
size_t my_strcspn(const char *str, const char *reject) {
    const char *p, *r;
    size_t count = 0;

    for (p = str; *p != '\0'; ++p) {
        for (r = reject; *r != '\0'; ++r) {
            if (*p == *r)
                return count;
        }
        ++count;
    }

    return count;
}

// Implementación artesanal de strcat
// Entrada: dos cadenas de caracteres
// Salida: concatenación de las dos cadenas
char* my_strcat(char* dest, const char* src) {
	// Mover el puntero al final de la cadena destino
	while (*dest != '\0') {
		dest++;
	}

	// Copiar cada caracter de la cadena fuente a la cadena destino
	while (*src != '\0') {
		*dest++ = *src++;
	}

	// Añadir el terminador nulo al final de la cadena destino
	*dest = '\0';
	return dest;
}

// Función auxiliar para obtener la longitud de una cadena
// Entrada: una cadena de caracteres
// Salida: tamaño de la cadena cómo entero
int my_strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


// Funciones de CSV
// ----------------------------

// Entrada: nombre del archivo y estructura donde se almacenarán los datos
// Salida: void, hace un proceso.
// Función que lee un archivo CSV y almacena las líneas en una estructura
void read_csv(const char *filename, CSVData *data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    // Inicializar contador de lineas
    data->line_count = 0;

    // Leer cada linea del archivo
    while (fgets(line, sizeof(line), file)) {
        // Eliminar el salto de línea al final de la línea
        line[my_strcspn(line, "\n")] = 0;

        // Copiar la linea al arreglo de lineas de la estructura
        my_strcpy(data->lines[data->line_count], line);
        data->line_count++;
    }

    // Cerrar el archivo
    fclose(file);
}

void read_csv_from_stream(FILE *stream, CSVData *data) {
    data->line_count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), stream)) {
        strcpy(data->lines[data->line_count], line);
        data->line_count++;
    }
}

// Función que muestra las líneas almacenadas del archivo
// Netamente para revisar si es que se leyo correctamente
// Entrada: estructura con los datos
// Salida: void, imprime en consola
void print_csv(const CSVData *data) {
    for (int i = 0; i < data->line_count; ++i) {
        printf("%s\n", data->lines[i]);
    }
}

int* reverse_array(int* array, int size){
	int* reversed_array = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++){
		reversed_array[i] = array[size - i - 1];
	}
	return reversed_array;
}

// Funcion para tomar un arreglo de caracteres separados por coma y transformarlos a arreglo de enteros
// Entrada: arreglo de caracteres, arreglo de enteros, puntero a entero
// Salida: void, quedan almacenados en output y count
void transform_string_to_array(const char input[], int output[], int *count) {
    int i = 0, j = 0, num = 0;
    int is_negative = 0;
    *count = 0;

    while (input[i] != '\0') {
        // Reiniciar número y bandera de negativo para el próximo número
        num = 0;
        is_negative = 0;

        // Comprobar si hay signo negativo
        if (input[i] == '-') {
            is_negative = 1;
            i++;
        }

        while (input[i] >= '0' && input[i] <= '9') {
            // Caso para números con más de un dígito
            num = num * 10 + (input[i] - '0');
            i++;
        }

        // Aplicar signo negativo si es necesario
        if (is_negative) {
            num = -num;
        }

        // Almacenar número en el arreglo de salida
        output[j++] = num;

        // Incrementar contador
        (*count)++;

        // Comprobar si hay una coma, si la hay, saltarla
        if (input[i] == ',') {
            i++;
        }
    }
}
