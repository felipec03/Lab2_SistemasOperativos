// Archivos de cabezera para "contrato" de cut y archivos
#include "cut.h"

// Funcion para separar las columnas de un archivo CSV en una estructura CSVColumns
// Entrada: Puntero a la estructura CSVData, delimitador, puntero donde se quiera guardar output
// Salida: void, queda como parametro de entrada la estructura CSVColumns con las columnas separadas
void split(CSVData *data, const char delimiter, CSVColumns *columns_data) {
	// IF resumido para el caso de que el delimitador sea '\0'
    char actual_delimiter = delimiter == '\0' ? '\t' : delimiter;

    // Inicializamos el contador de líneas
    columns_data->line_count = data->line_count;
    // Asignamos memoria para columnas y column_count
    columns_data->columns = (char ***)malloc(sizeof(char **) * data->line_count);
    columns_data->column_count = (int *)malloc(sizeof(int) * data->line_count);

    // Recorremos las lineas del archivo
    for (int i = 0; i < data->line_count; i++) {
    	// Variables para la linea actual, largo de linea y flag delimitador
        char *line = data->lines[i];
        int line_len = 0;
        int has_delimiter = 0;

        // Contamos los caracteres de la línea hasta el final
        while (line[line_len] != '\0') {
            if (line[line_len] == actual_delimiter) {
            	// Detectamos la presencia del delimitador
                has_delimiter = 1;
            }
            line_len++;
        }

        // Casos bordes
        if (!has_delimiter) {
            // Si no se encontró el delimitador, tratamos toda la línea como una sola columna
            columns_data->columns[i] = (char **)malloc(sizeof(char *));
            columns_data->columns[i][0] = (char *)malloc(sizeof(char) * (line_len + 1));
            // Se copia la línea completa
            for (int j = 0; j < line_len; j++) {
                columns_data->columns[i][0][j] = line[j];
            }
            columns_data->columns[i][0][line_len] = '\0';
            // Solo una columna
            columns_data->column_count[i] = 1;
        } else {
            // Aquí haces la separación normal si se encontró el delimitador
            char **columns = (char **)malloc(sizeof(char *) * line_len);
            int col_count = 0;
            int start = 0;

            // Iteramos sobre la línea para separar las columnas
            for (int j = 0; j <= line_len; j++) {
                // Si encontramos el delimitador o el final de la línea, separamos
                if (line[j] == actual_delimiter || line[j] == '\0') {
                    int word_len = j - start;
                    columns[col_count] = (char *)malloc(sizeof(char) * (word_len + 1));
                    // Para cada columna copiamos contenido
                    for (int k = 0; k < word_len; k++) {
                        columns[col_count][k] = line[start + k];
                    }
                    columns[col_count][word_len] = '\0';
                    col_count++;
                    start = j + 1;
                }
            }
            columns_data->columns[i] = columns;
            columns_data->column_count[i] = col_count;
        }
    }
}

// Funcion cut que selecciona columnas objetivo de un arreglo de lineas y columnas
// Entrada: columns_data: datos de las columnas, cobj: indices de las columnas objetivo, num_cobj: numero de columnas objetivo
// Salida: result: datos cortados.
char ***cut(CSVColumns *columns_data, int *cobj, int num_cobj) {
    int num_lines = columns_data->line_count;

    // Crear el resultado que contendra las columnas seleccionadas
    char ***result = (char ***)malloc(num_lines * sizeof(char **));

    // Iterar sobre cada línea
    for (int i = 0; i < num_lines; i++) {
    	// Almacenar las columnas seleccionadas por linea
        result[i] = (char **)malloc(num_cobj * sizeof(char *));
        // Iterar sobre indices de las columnas objetivo
        for (int j = 0; j < num_cobj; j++) {
        	// Indice de la columna que queremos extraer
         	// Desplazada por 1 para que no haya desfase con indice
            int col_index = cobj[j] - 1;

            // Verificar que el índice de la columna objetivo exista en esta línea
            if (col_index < columns_data->column_count[i]) {
                // Si la columna existe, asignarla al resultado
                // Largo de la columna
                int len = my_strlen(columns_data->columns[i][col_index]);
                // Asignar memoria
                result[i][j] = (char *)malloc((len + 1) * sizeof(char));
                // Copiar contenido con funcion artesanal
                my_strcpy(result[i][j], columns_data->columns[i][col_index]);
            } else {
                // Si columna no existe => asignar un espacio vacio
                result[i][j] = (char *)malloc(2 * sizeof(char));
                // Cadena vacia, end of file
                result[i][j][0] = '\0';
            }
        }
    }
    return result;
}

// Función para escribir el resultado de cut en un archivo CSV
// Entrada: cut_data: datos cortados, num_lines: número de líneas, num_cols: número de columnas, filename: nombre del archivo, delimiter: delimitador
// Salida: void, procesa un archivo de texto con los datos cortados
void out(char ***cut_data, int num_lines, int num_cols, FILE *output, char delimiter) {
    for (int i = 0; i < num_lines; i++) {
        for (int j = 0; j < num_cols; j++) {
            fprintf(output, "%s", cut_data[i][j]);
            if (j < num_cols - 1) {
                fprintf(output, "%c", delimiter);
            }
        }
        fprintf(output, "\n");
    }
}

// funcion para copiar un archivo a otro
// Entrada: file1: archivo de entrada en modo lectura, file2: archivo de salida en modo escritura
// Salida: void, copia el contenido de un archivo a otro, file1->file2
void copyarch(FILE *file1, FILE *file2) {
    char c;
    // Mientras no se acabe el archivo, copiar caracter por caracter
    while ((c = fgetc(file1)) != EOF) {
        // de biblioteca iostream
        fputc(c, file2);
    }
}

// Ejecucion principal del programa
int main(int argc, char *argv[]) {
    int opt;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;
    char* delimitador = NULL;
    char* stringColumnas = NULL;

    while((opt = getopt(argc, argv, "d:c:i:o:")) != -1){
        switch(opt){
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 'd':
                delimitador = optarg;
                break;
            case 'c':
                stringColumnas = optarg;
                break;
            default:
                abort();
        }
    }

    if (!stringColumnas){
        fprintf(stderr, "Error: Columns not specified\n");
        return 1;
    }

    CSVData data;
    if (archivoEntrada){
        read_csv(archivoEntrada, &data);
    } else {
        read_csv_from_stream(stdin, &data);
    }

    char delimiter = (delimitador && delimitador[0]) ? delimitador[0] : ',';

    CSVColumns columns_data;
    split(&data, delimiter, &columns_data);

    int columnasObjetivo[100];
    int num_cobj = 0;
    transform_string_to_array(stringColumnas, columnasObjetivo, &num_cobj);
    int *columnasInvertidas = reverse_array(columnasObjetivo, num_cobj);

    char ***cut_data = cut(&columns_data, columnasInvertidas, num_cobj);

    FILE *output = stdout;
    if (archivoSalida){
        output = fopen(archivoSalida, "w");
        if (!output){
            perror("Error opening output file");
            return 1;
        }
    }

    out(cut_data, columns_data.line_count, num_cobj, output, delimiter);

    if (archivoSalida){
        fclose(output);
    }

    return 0;
}