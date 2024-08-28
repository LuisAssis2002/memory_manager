#include "memory_manager.h"

//coleta os dados de entrada e os armazena na variavel report, além de inicializar a mesma
void getParameters(int argc, char** argv, struct statistics* report) {
    if (argc == 5) {    //confere a quantidade de parametros passadas
        //determina o algoritimo de substituição
        if(strcmp(argv[1], "lru") == 0) report->s_alg = 0;
        if(strcmp(argv[1], "nru") == 0) report->s_alg = 1;
        if(strcmp(argv[1], "segunda_chance") == 0) report->s_alg = 2;
        
        //aloca e armazena o nome do arquivo .log
        report->file_name = (char*) malloc(15 * sizeof(char));
        strcpy(report->file_name, argv[2]);

        //guarda o tamando da memória e das páginas
        report->page_size = atoi(argv[3]);
        report->memory_size = atoi(argv[4]);

    } else {        //trata o erro na entrada e finaliza o programa
        printf("Parametros faltantes na inicializacao!\n");
        exit(0);
    }
    //inicializa os contadores
    report->dirty_pages = 0;
    report->reads = 0;
    report->writes = 0;
}

//trata a instrução inicial e determina o endereço na tabela virtual
char* getAddress(char* inst, int a_b_num) {
    char* address;
    address = binaryCreator(address, a_b_num); //cria um binário com tamanho dos endereços da tabela virtual
    for (int i = 0; i < a_b_num; i++) address[i] = inst[i]; //copia os bits que representão o endereço virtual
    return address;
}

//decodifica a string contendo a instrução, e retorna uma instruction
void decoder(struct instruction* inst, char* code, int a_b_num) {
    char *hex;
    double dec = 0;
    char *aux = binaryCreator(aux, I_B_NUM);
    //free(inst->str);        //libera da memória a instrução anterior
    hex = malloc(MAX_LENGHT * sizeof(char));
    int i = 0;
    while(code[i] != ' '){
        hex[i] = code[i];
        i++;
    }
    //realiza a converção de hexadecimal para binário
    hex2dec(hex, &dec);
    dec2bin(dec, aux);

    inst->str = getAddress(aux, a_b_num); //coleta o endereço na tabela virtual
    while(code[i] == ' ') i++;
    inst->type = code[i];   //coleta o tipo de instrução
}

//cria a página e inicializa os valores
struct page* createPage(char* inst, double value) {
    struct page* p = (struct page*) malloc(sizeof(struct page));
    p->virtualIndex = inst;
    p->realIndex = -1; 
    p->M = 0;
    p->R = 0;
    return p;
}

//algoritimo de subistituição lru
void lru(struct hash* m, struct page* e_p, struct statistics* report) {
    struct rec* aux;
    struct page* s_p;
    struct page* p;
    double l_time = INT_MAX;
    double time;
    int index = 0;

    //roda a memória principal buscando o menor tempo (mais antigo)
    for (int i = 0; i < m->key; i++){
        aux = &m->table[i];
        p = (struct page*)aux->object;
        time = ((double)p->lastUse/(CLOCKS_PER_SEC/1000));
        if(time < l_time) {
            l_time = time;
            s_p = p;
            index = i;
        }
    }

    //realiza a subistituição na memória
    e_p->realIndex = index;
    s_p->realIndex = -1;
    e_p->R = 1;
    m->table[index].object = e_p;

    //verifica se a página sub. está "suja"
    if(s_p->M) {
        report->dirty_pages++;
        s_p->M = 0;  
    }
    report->writes++;
}

//algoritimo de subistituição nru
void nru(struct hash* m, struct page* e_p, struct statistics* report) {
    struct rec* aux;
    struct page* s_p = NULL;
    struct page* p;
    double time;

    //páginas destinadas a guardar as págs. de cada classe e seus index
    struct page *p0, *p1, *p2, *p3;
    int index = 0, index0 = 0, index1 = 0, index2 = 0, index3 = 0;
    p0 = NULL;
    p1 = NULL;
    p2 = NULL;
    p3 = NULL;

    //roda a memória principal buscando a página de menor classe
    for (int i = 0; i < m->key; i++){
        aux = &m->table[i];
        p = (struct page*)aux->object;

        if((p->R == 0) && (p->M == 0)) { //Classe 0
            index0 = i;
            p0 = p;
            break;
        }
        if((p->R == 1) && (p->M == 0) && (p1 == NULL)) { //Classe 1
            index1 = i;
            p->R = 0;
            p->lastUse = clock();
            p1 = p;
        }
        if((p->R == 0) && (p->M == 1) && (p2 == NULL)) { //Classe 2
            index2 = i;
            p2 = p;
        }
        if((p->R == 1) && (p->M == 1) && (p3 == NULL)) { //Classe 3
            index3 = i;
            p->R = 0;
            p->lastUse = clock();
            p3 = p;
        }
    }

    //Seleciona a página de menor classe
    if (p0 != NULL) {
        s_p =  p0;
        index = index0;
    } else if (p1 != NULL) {
        s_p =  p1;
        index = index1;
    } else if (p2 != NULL) {
        s_p =  p2;
        index = index2;
    } else if (p3 != NULL) {
        s_p =  p3;
        index = index3;
    }
    
    e_p->realIndex = index;
    s_p->realIndex = -1;
    e_p->R = 1;
    m->table[index].object = e_p;

    if(s_p->M) {
        report->dirty_pages++;
        s_p->M = 0;  
    }
    report->writes++;
}

//algoritimo de subistituição second_chance
void sc(struct hash* m, struct page* e_p, struct statistics* report) {
    struct rec* aux;
    struct page* s_p = NULL;
    struct page* p;
    double l_time = INT_MAX;
    double time;
    int index = 0;

    do {
        //roda a memória principal buscando a página mais antiga não referenciada. R != 1
        for (int i = 0; i < m->key; i++){
            aux = &m->table[i];
            p = (struct page*)aux->object;
            if (p->R != 1) {
                time = ((double)p->lastUse/(CLOCKS_PER_SEC/1000));
                if(time < l_time) {
                    l_time = time;
                    s_p = p;
                    index = i;
                }
            } else {                    //caso a pág. tenha sido referenciada
                p->R = 0;               //configura como nao referenciada
                p->lastUse = clock();   //e atualiza o tempo
            }
        }
    } while (s_p == NULL);  //repete a busca caso todas as págs. estejam referenciadas

    e_p->realIndex = index;
    s_p->realIndex = -1;
    e_p->R = 1;
    m->table[index].object = e_p;

    if(s_p->M) {
        report->dirty_pages++;
        s_p->M = 0;  
    }
    report->writes++;
}

//imprimi uma instrução
void printInst(struct instruction* inst) {
    printf("Instruction: %s - Type: %c", inst->str, inst->type);
}

//imprimi os dados de uma página
void printPage(struct page* p) {
    printf("    %s, %f, %f, %d, %d\n", p->virtualIndex, p->realIndex, ((double)p->lastUse/(CLOCKS_PER_SEC/1000)), p->M, p->R);
}

//imprimi um tabela, podendo ser a memória ou a tabela virtual
void printTable(struct hash* virtualTable) {
    struct rec* aux;
    struct page* p;
    for (int i = 0; i < virtualTable->key; i++){
        printf("\n%d :\n", i);
        aux = &virtualTable->table[i];
        if(aux->object != NULL) {
            while(aux->next != NULL) {
                p = (struct page*)aux->object;
                printPage(p);
                aux = aux->next;
            }
            p = (struct page*)aux->object;
            printPage(p);

        }
    }
}

//imprimi o report
void printReport(struct statistics* report) {
    printf("Arquivo de entrada: %s\n", report->file_name);
    printf("Tamanho da memoria: %d KB\n", report->memory_size);
    printf("Tamanho das paginas: %d KB\n", report->page_size);
    if(report->s_alg == 0) printf("Tecnica de reposicao: lru\n");
    if(report->s_alg == 1) printf("Tecnica de reposicao: nru\n");
    if(report->s_alg == 2) printf("Tecnica de reposicao: segunda_chance\n");
    printf("Paginas lidas: %d\n", report->reads);
    printf("Paginas escritas: %d\n", report->writes);
    printf("Paginas sujas: %d\n", report->dirty_pages);
}