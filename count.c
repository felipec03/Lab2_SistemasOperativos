#include "count.h"

// Función que cuenta el número de líneas en el archivo
// Entrada: Puntero a la estructura CSVData
// Salida: Numero de líneas en el archivo
int contar_lineas(CSVData *data){
    return data->line_count;
}

// Funcion que cuenta el numero de caracteres en todas las líneas
// Entrada: Puntero a la estructura CSVData
// Salida: Numero de caracteres en todas las lineas
int contar_caracteres(CSVData *data){
    int contador = 0;
    for(int i = 0; i < data->line_count; i++){
        for(int j = 0; data->lines[i][j] != '\0'; j++){
            contador++;
        }
    }
    return contador;
}

int main(int argc, char *argv[])
{
    int opt;
    int opcionC = 0;
    int opcionL = 0;
    char* archivoEntrada = NULL;
    char* archivoSalida = NULL;

    while((opt = getopt(argc, argv, "CLi:o:")) != -1)
    {
        switch(opt)
        {
            case 'i':
                archivoEntrada = optarg;
                break;
            case 'o':
                archivoSalida = optarg;
                break;
            case 'C':
                opcionC = 1;
                break;
            case 'L':
                opcionL = 1;
                break;
            default:
                abort();
        }
    }

    CSVData data;
    if (archivoEntrada)
    {
        read_csv(archivoEntrada, &data);
    }
    else
    {
        read_csv_from_stream(stdin, &data);
    }

    int lineas = data.line_count;
    int caracteres = contar_caracteres(&data);

    FILE *output = stdout;
    if (archivoSalida)
    {
        output = fopen(archivoSalida, "w");
        if (!output)
        {
            perror("Error opening output file");
            return 1;
        }
    }

    if(opcionC && !opcionL){
        fprintf(output, "%d\n", caracteres);
    } else if(opcionL && !opcionC){
        fprintf(output, "%d\n", lineas);
    } else if(opcionL && opcionC){
        fprintf(output, "%d %d\n", lineas, caracteres);
    }

    if (archivoSalida)
    {
        fclose(output);
    }

    return 0;
}