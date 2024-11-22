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
	// Variable propia de getopt y unistd
    int opt;

    // Variables para las opciones de flags
    int opcionC = 0;
    int opcionL = 0;
    // Variable para el nombre del archivo
    char* archivo = NULL;

    // Ciclo para leer las opciones de los flags
    while((opt = getopt(argc, argv, "CLi:")) != -1)
    {
        switch(opt)
        {
        	// Opcion para el nombre del archivo
        	case 'i':
                archivo = optarg;
                break;

            // Opcion para contar el numero de caracteres
            case 'C':
                opcionC = 1;
                break;

            // Opcion para contar el numero de lineas
            case 'L':
            	opcionL = 1;
                break;

            // En cualquier otro caso, abortar
            default:
	            abort();
        }
    }

    // Leer el archivo CSV y almacenar las líneas en la estructura `data`
    CSVData data;
    read_csv(archivo, &data);

    // Sacando el número de líneas y caracteres por medio de la estructura
    // Además de contar caracteres declarado al comienzo del programa
    int lineas = data.line_count;
    int caracteres = contar_caracteres(&data);

    // Opciones de impresión por flags
    if((opcionC == 1) && (opcionL == 0)){
    	printf("%d\n", caracteres);
    }

    if ((opcionL == 1) && (opcionC == 0)){
		printf("%d\n", lineas);
	}

    if((opcionL == 1) && (opcionC == 1)){
    	printf("%d %d\n", lineas, caracteres);
    }

    return 0;
}
