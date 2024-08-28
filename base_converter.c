#include "base_converter.h"

void hex2dec(char *hex, double *dec) {
    int tam = strlen(hex) - 1;
    int aux;
    for(int i = 0; i < strlen(hex); i++, tam--) {
        if ((hex[i] >= '0') && (hex[i] <= '9')) {
            aux = hex[i] - '0';
            *dec += aux * (double) pow(16, tam);
        }
        if (hex[i] == 'a') *dec += 10 * (double) pow(16, tam);
        if (hex[i] == 'b') *dec += 11 * (double) pow(16, tam);
        if (hex[i] == 'c') *dec += 12 * (double) pow(16, tam);
        if (hex[i] == 'd') *dec += 13 * (double) pow(16, tam);
        if (hex[i] == 'e') *dec += 14 * (double) pow(16, tam);
        if (hex[i] == 'f') *dec += 15 * (double) pow(16, tam);
    }
}

void dec2bin(double dec, char *bin) {
    int tam  = strlen(bin) - 1;
    int aux;
    for (int i = 0; i < strlen(bin); i++, tam--) {
        if (dec == 0) {
            bin[tam] = dec + '0';
        } else {
            aux = (int)dec % 2;
            bin[tam] = aux + '0';
            dec = dec / 2;
        }
    }
}

void bin2dec(char *bin, double *dec) {
    int tam = strlen(bin) - 1;
    double aux;
    for(int i = 0; i < strlen(bin); i++, tam--) {
        aux = bin[i] - '0';
        *dec += aux * (double) pow(2, tam);
    }
}

char* binaryCreator(char *bin, int b_num) {
    bin = (char*) malloc(b_num * sizeof(char));
    for (int i = 0; i < b_num; i++) bin[i] = '0';
    return bin;
}

/*
void main() {
    char str[] = "190a7c20";
    char bin[] = "00000000000000000000000000000000";
    int dec = 0;
    int d =0;
    hex2dec(str, &dec);
    dec2bin(dec, bin);
    bin2dec(bin, &d);
    printf("%d - %s - %d\n", dec, bin, d);
}
*/