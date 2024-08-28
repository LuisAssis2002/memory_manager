#include "file.h"

//abre um arquivo e retorna um ponteiro para o mesmo
FILE* open_file(FILE* arq, char* fn) {
    arq = fopen(fn, "r");
    if(arq == NULL) {           //caso ocorra um erro de leitura
        printf("Erro na leitura do arquivo.\n");
    }
    return arq;
}

//armazena em um array de strings contendo as instruções de um programa
char** SPM(FILE* arq) {
    char** p_array;
    char buffer[500];
    int size = 1, index = 0;

    p_array = (char**) malloc(INTRUCTIONS_LIMIT * sizeof(char*));
    p_array[size-1] = malloc(MAX_LENGHT * sizeof(char));

    while(fgets(buffer, 500, arq) != NULL) { //coleta 500 caracters do arquivo e guarda no buffer
        for(int i = 0; i < strlen(buffer); i++) {
            if((buffer[i] == '\n') || (buffer[i] == '\0') || (buffer[i] == EOF)) {  //caso chegue a uma quebra de linha ou final do arquivo
                size++;
                i++;     
    
                if (buffer[i] != EOF) {     
                    p_array[size-1] = malloc(MAX_LENGHT * sizeof(char));    //aloca memoria da proxima instrução
                    index = 0;    //reseta o indice dos caracters da instrução
                }
                break;
            }
            p_array[size - 1][index] = buffer[i];
            index++;
        }
    }
    return p_array;
}
