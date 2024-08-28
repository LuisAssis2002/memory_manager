#include <stdio.h>
#include <stdlib.h>

//nós da hash
struct rec {    
    void* object;       //dado
    int id;             //id (valor) do dado usado no chaveamento
    struct rec* next;   //ponteiro para o próximo nó
};

//tablea hash
struct hash {
    struct rec* table;  //ponteiro para os nós iniciais de cada linha
    int key;            //chave usado no caulculo de referenciamento dos dados
};

//aloca e inicializa uma hash de tamanho table_size
void createHash(struct hash* hash, int table_size);

//inseri um object na hash, baseado em seu valor object_index
void insertHash(struct hash* hash, void* object, int object_index); 

//libera a hash da memória
void freeHash(struct hash* hash);

//procura um objeto pelo id na memória, o retornando caso exista e NULL caso não 
void* searchId(struct hash* hash, int id);
