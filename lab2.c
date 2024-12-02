#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char** splitString(const char* input, int* num_tokens) {
    // First, count the number of tokens
    int token_count = 1;
    int input_length = 0;
    
    // Count the number of '|' characters to determine token count
    while (input[input_length] != '\0') {
        if (input[input_length] == '|') {
            token_count++;
        }
        input_length++;
    }
    
    // Allocate memory for the array of string pointers
    char** tokens = (char**)malloc(token_count * sizeof(char*));
    if (tokens == NULL) {
        *num_tokens = 0;
        return NULL;
    }
    
    // Allocate memory for each token and copy characters
    int current_token = 0;
    int start = 0;
    
    for (int i = 0; i <= input_length; i++) {
        // If we've found a '|' or reached the end of the string
        if (input[i] == '|' || input[i] == '\0') {
            // Allocate memory for the current token
            int token_length = i - start;
            tokens[current_token] = (char*)malloc((token_length + 1) * sizeof(char));
            
            if (tokens[current_token] == NULL) {
                // Free previously allocated memory if allocation fails
                for (int j = 0; j < current_token; j++) {
                    free(tokens[j]);
                }
                free(tokens);
                *num_tokens = 0;
                return NULL;
            }
            
            // Copy characters to the token
            for (int j = 0; j < token_length; j++) {
                tokens[current_token][j] = input[start + j];
            }
            tokens[current_token][token_length] = '\0';
            
            // Move to next token
            current_token++;
            start = i + 1;
        }
    }
    
    // Set the number of tokens
    *num_tokens = token_count;
    
    return tokens;
}


// Conectar las 3 operaciones por medio de fork() y pipe() de m
int main(int argc, char *argv[]){

    // Se hace un getopt para simular pipes para las opearciones count, srep y cut
    // El formato de comando de ejemplo debería ser
    // $ ./lab2 ./srep -i input.txt -s / -S \ | cut -d : -c 2,4
    // Se toma cómo argumento de argv[1] el comando completo y se hace un split para obtener los comandos

    if(argc!=2) {
        printf("Error fatal en input...\n");
        exit(1); 
    }
    const char* comando = argv[1];
    //printf("Comando inputeado: %s \n", comando);

    int num_comandos;
    char** comandos = splitString(comando, &num_comandos);

    printf("Comandos: \n");
    for(int i = 0; i<num_comandos; i++){
        printf("Comando %d: %s\n", i, comandos[i]);
    }

    // Se crean los pipes
    int fd[2];
    if(pipe(fd) == -1){
        perror("Error en pipe");
        exit(-1);
    }

    //free(comando);
}