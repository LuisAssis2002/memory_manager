#include "memory_manager.h"

void main(int argc, char *argv[]) {
    //inicializa as variáveis principais
    clock_t init_t;     //tempo de início do programa
    FILE* arq;          
    struct instruction inst;
    struct statistics report;
    struct page* p;
    struct hash h;      //tabela virtual
    struct hash memory; //mapeamento da memória principal

    //inicializa variáveis aux
    char** array;       //conteúdo do arquivo
    double value = 0;   //guarde os endereços em decimais
    int cont = 0;       //contador

    getParameters(argc, argv, &report);

    int page_num = report.memory_size / report.page_size;   //calcula o número de páginas da memória
    int d_b_num = (int) log2(page_num) + 1;     //bits de deslocamento
    int a_b_num = I_B_NUM - d_b_num;            //bits de endereçamento

    createHash(&h, HASH_SIZE);
    createHash(&memory, page_num);
    arq = open_file(arq, report.file_name);    //abri o arquivo inicial
    array = SPM(arq);           //extrai as instruções

    printf("Executando o simulador...\n");

    for (int i = 0; strlen(array[i]) > 0; i++) {
        value = 0;
        decoder(&inst,  array[i], a_b_num); //decodifica o texto em instrução 
        bin2dec(inst.str, &value);      //guarda o endereço em decimal da tabela virtual
        p = (struct page*) searchId(&h, value); //encontra página da instrução
        
        if(p == NULL) {     //caso a página ainda não exista aloca ela
            p = createPage(inst.str, value);
            insertHash(&h, p, i);
        } 
        if(p->realIndex > -1){  //caso a página exista e já esta na memória
            p->R = 1;
            report.reads++;
        } else if (cont < page_num){    //caso a memória ainda não esteja cheia
            p->realIndex = cont;
            p->R = 1;  
            report.writes++;
            cont++;
            insertHash(&memory, p, cont);
        } else {    //caso a memória esteja cheia, chama o alg. de sub.
            if (report.s_alg == 0) lru(&memory, p, &report);
            if (report.s_alg == 1) nru(&memory, p, &report);
            if (report.s_alg == 2) sc(&memory, p, &report);
        }
        if(inst.type == 'W') p->M = 1;
        p->lastUse = clock();
    }

    //printTable(&h);
    //printTable(&memory);
    printReport(&report);
    //p = (struct page*) searchId(&h, 39);
    //printPage(p);
    //freeHash(&h);
}