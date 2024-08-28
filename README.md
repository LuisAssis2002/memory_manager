# memory_manager
Simulador de gerenciamento de memória

O programa deverá ser iniciado com quatro argumentos:
./tp2virtual lru arquivo.log 4 128

Esses argumentos representam, pela ordem:
  1. o algoritmo de substituição a ser usado (lru, nru ou segunda chance);
  2. o arquivo contendo a sequência de endereços de memória acessados (arquivo.log, nesse exemplo);
  3. o tamanho de cada página/quadro de memória, em kilobytes - faixa de valores razoáveis: de 2 a 64;
  4. o tamanho total da memória física disponível para o processo, também em kilobytes - faixa de valores
     razoáveis: de 128 a 16384 (16 MB).
