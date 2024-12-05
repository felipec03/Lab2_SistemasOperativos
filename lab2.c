#define MAX_ARGS 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Función para separar una cadena de caracteres por pipes
// Entrada: Una cadena de caracteres y un puntero a un entero
// Salida: Un arreglo de strings (puntero a caracter) y la cantidad de strings que fueron separadas por pipes
char** splitString(const char* input, int* num_tokens) {
    // Variables para guardar el número de "tokens" y la longitud de la cadena de entrada
    int token_count = 1;
    int input_length = 0;
    
    // Se cuentan los pipes para determinar el número de tokens
    while (input[input_length] != '\0') {
        if (input[input_length] == '|') {
            token_count++;
        }
        input_length++;
    }
    
    // Se guarda memoria para las strings
    char** tokens = (char**)malloc(token_count * sizeof(char*));

    // Si está vacío, se retorna NULL
    if (tokens == NULL) {
        *num_tokens = 0;
        return NULL;
    }
    
    // Se guarda memoria para cada token
    int current_token = 0;
    // Auxiliar para guardar la posición de inicio de cada token
    int start = 0;
    
    for (int i = 0; i <= input_length; i++) {
        // Si es que se encuentra un pipe o el final de la cadena
        if ((input[i] == '|' && input[i+1] == ' ')|| input[i] == '\0') {
            // Guardamos memoria para el token
            int token_length = i - start;
            tokens[current_token] = (char*)malloc((token_length + 1) * sizeof(char));
            
            // Caso perdido
            if (tokens[current_token] == NULL) {
                // Si falla, se libera la memoria.
                for (int j = 0; j < current_token; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                *num_tokens = 0;
                return NULL;
            }
            
            // Se copia el token a la memoria
            for (int j = 0; j < token_length; j++) {
                tokens[current_token][j] = input[start + j];
            }
            tokens[current_token][token_length] = '\0';
            
            // Se va al siguiente token
            current_token++;
            start = i + 1;
        }
    }
    
    // Se devuelve además, la cantidad de strings que fueron separadas por pipes.
    *num_tokens = token_count;
    
    return tokens;
}

void parseCommand(char* cmd, char** args) {
    int i = 0;
    args[i] = strtok(cmd, " ");
    while(args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

// Conectar las 3 operaciones por medio de fork() y pipe() de m
int main(int argc, char *argv[]){

    // El formato de comando de ejemplo debería ser
    // $ ./lab2 "./srep -i input.txt -s / -S \ | /cut -d : -c 2,4"
    // Se toma cómo argumento de argv[1] el comando completo y se hace un split para obtener los comandos

    if(argc!=2) {
        printf("Error fatal en input...\n");
        exit(1); 
    }
    const char* comando = argv[1];
    //printf("Comando inputeado: %s \n", comando);

    int num_comandos;
    char** comandos = splitString(comando, &num_comandos);

    // Se "limpian" las strings de después de los pipes que tiene un espacio extra.
    for(int i = 1; i<num_comandos; i++){
        comandos[i] = memmove(comandos[i], comandos[i]+1, strlen(comandos[i]));
    }
    
    printf("Comandos: \n");
    for(int i = 0; i<num_comandos; i++){
        printf("%s\n", comandos[i]);
    }

    // La idea es ir por cada proceso en el arreglo de comandos y hacer un fork() y pipe() para conectarlos
    // Por lo que la secuencia de pasos a seguir es:
    // - Crear el proceso
    // - Crear el pipe
    // - Ejecutar el proceso con sus respectivos argumentos de getopt
    // - Conectar los pipes
    // - Esperar a que el proceso termine

    int num_pipes = num_comandos - 1;
    int fd[2 * num_pipes]; // File descriptors for pipes

    // Create all necessary pipes
    for(int i = 0; i < num_pipes; i++) {
        if(pipe(fd + i*2) < 0) {
            perror("Failed to create pipe");
            exit(1);
        }
    }

    // Loop through commands and fork processes
    for(int i = 0; i < num_comandos; i++) {
        pid_t pid = fork();

        // Proceso hijo
        if(pid == 0) {

            // Redirect input if not first command
            if(i != 0) {
                dup2(fd[(i-1)*2], 0);
            }

            // Redirect output if not last command
            if(i != num_comandos - 1) {
                dup2(fd[i*2 + 1], 1);
            }

            // Close all pipe file descriptors in child
            for(int j = 0; j < 2 * num_pipes; j++) {
                close(fd[j]);
            }

            // Parse command and execute
            char *args[MAX_ARGS];
            parseCommand(comandos[i], args);
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        }
    }

    // Parent process closes all pipes
    for(int i = 0; i < 2 * num_pipes; i++) {
        close(fd[i]);
    }

    // Parent waits for all children
    for(int i = 0; i < num_comandos; i++) {
        wait(NULL);
    }
}