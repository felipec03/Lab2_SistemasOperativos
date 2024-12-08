#include <stdio.h> // Incluye la librería estándar de E/S
#include <stdlib.h> // Incluye la librería estándar
#include <string.h> // Incluye la librería para manejo de strings
#include <unistd.h> // Incluye la librería para llamadas al sistema
#include <sys/wait.h> // Incluye la librería para llamadas al sistema wait
#include <fcntl.h> // Incluye la librería para llamadas al sistema open
#define MAX_ARGS 20

// Función para separar los comandos por pipes
char** splitString(const char* input, int* num_tokens) {
    int token_count = 1;
    int input_length = 0;
    
    // Contar los pipes
    while (input[input_length] != '\0') {
        if (input[input_length] == '|') {
            token_count++;
        }
        input_length++;
    }
    
    // Asignar memoria para los tokens
    char** tokens = (char**)malloc(token_count * sizeof(char*));
    if (tokens == NULL) {
        *num_tokens = 0;
        return NULL;
    }

    int current_token = 0;
    int start = 0;
    for (int i = 0; i <= input_length; i++) {
        if ((input[i] == '|' && input[i+1] == ' ') || input[i] == '\0') {
            int token_length = i - start;
            tokens[current_token] = (char*)malloc((token_length + 1) * sizeof(char));
            if (tokens[current_token] == NULL) {
                for (int j = 0; j < current_token; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                *num_tokens = 0;
                return NULL;
            }
            memcpy(tokens[current_token], input + start, token_length);
            tokens[current_token][token_length] = '\0';
            current_token++;
            start = i + 1;
        }
    }

    *num_tokens = token_count;
    return tokens;
}

// Función para dividir los comandos en sus argumentos
// Entrada: cmd: comando a dividir, args: arreglo de argumentos
// Salida: void, modifica el arreglo de argumentos
void parseCommand(char* cmd, char** args) {
    int i = 0;
    args[i] = strtok(cmd, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2 || strlen(argv[1]) == 0) {
    fprintf(stderr, "Error: Entrada vacía o mal formada.\n");
    exit(1);
    }

    const char* command = argv[1];
    int num_commands;
    char** commands = splitString(command, &num_commands);
    if (commands == NULL) {
        fprintf(stderr, "Error: La cadena de comandos no se pudo dividir correctamente.\n");
        exit(1);
    }
    // #pipes = #comandos - 1, se trabajan de a pares cómo en diagrama del enunciado
    int num_pipes = num_commands - 1; 
    // Cantidad de descriptores de archivos respectivos a los pipes
    int fd[2 * num_pipes]; 

    // Crear pipes
    for (int i = 0; i < num_pipes; i++) {
        // En caso de error al crear el pipe
        if (pipe(fd + i * 2) < 0) {
            perror("Error: No se pudo crear el pipe");
            exit(1);
        }
    }

    // Se va a ejecutar un comando por cada iteración
    for (int i = 0; i < num_commands; i++) {
        // Crea un proceso hijo <=> comando a ejecutar
        pid_t pid = fork(); 
        if (pid == 0) { // ¿Estamos en proceso hijo?
            // Redirigir la entrada si no es el primer comando
            if (i != 0) {
                dup2(fd[(i - 1) * 2], 0); // Leer del pipe anterior
            }

            // Redirigir la salida si no es el último comando
            if (i != num_commands - 1) {
                dup2(fd[i * 2 + 1], 1); // Escribir en el siguiente pipe
            }

            // Cerrar todos los descriptores de archivo en el hijo
            for (int j = 0; j < 2 * num_pipes; j++) {
                close(fd[j]);
            }
            // Parsear el comando y ejecutar
            char* args[MAX_ARGS]; // Argumentos del comando
            parseCommand(commands[i], args); // Parsear el comando


            // Este trozo de código en general es para ejecutar los comandos sin necesidas del "./"
            char *path = getenv("PATH");

            // "String" donde se guarda el directorio del comando
            char new_path[1024];

            // Parte de stdio
            // Se usa para concatenar el directorio del comando con el directorio actual
            snprintf(new_path, sizeof(new_path), "%s:%s", path, ".");

            // Parte de la librería para UNIX
            // Se usa para cambiar el directorio del comando
            setenv("PATH", new_path, 1);

            // Ahora se ejecuta el comando de todas formas
            // En caso de error, se imprime un mensaje
            if(execvp(args[0], args) < 0) {
                perror("Error: No se pudo ejecutar el comando");
                exit(1);
            }

            perror("Error con execvp...");
            exit(1);
        }
    }

    // Cerrar todos los pipes en el padre
    for (int i = 0; i < 2 * num_pipes; i++) {
        close(fd[i]); // Cerrar el file descriptor
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < num_commands; i++) {
        wait(NULL); // Esperar a que un proceso hijo termine
    }

    // Liberar memoria
    for (int i = 0; i < num_commands; i++) {
        free(commands[i]); // Liberar la memoria de cada comando
    }
    free(commands); // Liberar la memoria de los comandos

    return 0;
}
