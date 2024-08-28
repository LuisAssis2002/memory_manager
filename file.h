#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGHT 30        //tamanho máximo de uma instrução
#define INTRUCTIONS_LIMIT 10000000 //limite de instruções por programa

//abre um arquivo e retorna um ponteiro para o mesmo
FILE* open_file(FILE* arq, char* fn);

//armazena em um array de strings contendo as instruções de um programa
char** SPM(FILE* arq);
