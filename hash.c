#include "hash.h"

void createHash(struct hash* hash, int table_size) {
    hash->table = (struct rec*) malloc(table_size * sizeof(struct rec));
    hash->key = table_size;
    for (int i = 0; i < hash->key; i++) {
        hash->table[i].object = NULL;
        hash->table[i].id = -1;
        hash->table[i].next = NULL;
    }
}

void insertHash(struct hash* hash, void* object, int object_index) {
    int index;
    struct rec* aux;
    index = object_index % hash->key;
    aux = &hash->table[index];
    if(aux->object != NULL) {
        while(aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = (struct rec*) malloc(sizeof(struct rec));
        aux = aux->next;
        aux->object = object;
        aux->id = object_index;
        aux->next = NULL;
    } else {
        aux->object = object;
        aux->id = object_index;
    }
}

void* searchId(struct hash* hash, int id) {
    int index; 
    struct rec* aux;
    index = id % hash->key;
    aux = &hash->table[index];
    if ((aux->id == id) && (aux->object != NULL)) return aux->object;
    while(aux->next != NULL) {
        aux = aux->next;
        if (aux->id == id) return aux->object;
    }
    return NULL;
}

void freeHash(struct hash* hash) {
    struct rec* aux1, *aux2;
    void* p;
    for (int i = 0; i < hash->key; i++){
        aux1 = &hash->table[i];
        printf("begin\n");
        if(aux1->object != NULL) {
            while(aux1->next != NULL) {
                aux2 = aux1->next;
                p = aux1->object;
                free(aux1);
                aux1 = aux2;
            }
            free(aux1);
            printf("fl\n");
        }
    }
}

/*
void main() {
    int tam = 200;
    int* list = (int*) malloc(tam * sizeof(int));
    struct hash h;
    createHash(&h, 50);

    for (int i = 0; i < tam; i++) {
        list[i] = i;
        insertHash(&h, &list[i], i);
    }

    freeHash(&h);
    free(list);
    //printHash(&h);
}
*/