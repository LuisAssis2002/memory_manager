#include "hash.h"
#include "base_converter.h"
#include "file.h"
#include "limits.h"
#include <time.h>

#define HASH_SIZE 5000  //tamanho da hash usada para alocar as paginas virtuais
#define I_B_NUM 32      //numero de bits para representar uma instrução

struct statistics {
    char* file_name;    //guarda o nome do arquivo .log
    int memory_size;    //guarda o tamanho destinado a memória principal
    int page_size;      //guarda o tamanho destinado a cada página
    int s_alg;          //guarda o algoritimo de subistituição. 0 - lru/ 1 - nru/ 2 - second_chance
    int reads;          //número de acessos corretos a memória
    int writes;         //número de page faults
    int dirty_pages;    //número de páginas sujas subistituidas e atualizadas no disco
};

struct page {
    char* virtualIndex; //índex da página virtual. Ex.: 0010
    double realIndex;   //índex da página na memória. Obs.: Páginas que não estão na memória recebem -1
    clock_t lastUse;    //último tempo de referenciamento
    int M;              //0 - Não modificado / 1 - Modificado
    int R;              //0 - Não referenciado / 1 - Referenciado
};

struct instruction{
    char type;  //Armazena o tipo da instrução
    char* str;  //Armazena o segundo termo da instrução
};

//coleta os dados de entrada e os armazena na variavel report, além de inicializar a mesma
void getParameters(int argc, char** argv, struct statistics* report);

//trata a instrução inicial e determina o endereço na tabela virtual
char* getAddress(char* inst, int a_b_num);

//decodifica a string contendo a instrução, e retorna uma instruction
void decoder(struct instruction* inst, char* code, int a_b_num);

//cria a página e inicializa os valores
struct page* createPage(char* inst, double value);

//algoritimo de subistituição lru
void lru(struct hash* m, struct page* e_p, struct statistics* report);

//algoritimo de subistituição nru
void nru(struct hash* m, struct page* e_p, struct statistics* report);

//algoritimo de subistituição second_chance
void sc(struct hash* m, struct page* e_p, struct statistics* report);

//imprimi uma instrução
void printInst(struct instruction* inst);

//imprimi os dados de uma página
void printPage(struct page* p);

//imprimi um tabela, podendo ser a memória ou a tabela virtual
void printTable(struct hash* virtualTable);

//imprimi o report
void printReport(struct statistics* report);