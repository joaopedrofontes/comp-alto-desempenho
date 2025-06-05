/*
Tarefa 1:
Implemente duas versões da
multiplicação de matriz por vetor (MxV)
em C: uma com acesso à matriz por
linhas (linha externa, coluna interna) e
outra por colunas (coluna externa, linha
interna). Meça o tempo de execução de
cada versão com uma função apropriada
e execute testes com diferentes
tamanhos de matriz. Identifique a partir
de que tamanho os tempos passam a
divergir significativamente e explique por
que isso ocorre, relacionando suas
observações com o uso da memória
cache e o padrão de acesso à memória.
*/




#include <stdio.h>
#include <time.h>

void multiplicacao_row_major(int linhas, int colunas, int matriz[linhas][colunas], int vetor[colunas], int resultado[linhas]) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            resultado[i] += matriz[i][j] * vetor[j];
        }
    }
}

void multiplicacao_column_major(int linhas, int colunas, int matriz[linhas][colunas], int vetor[colunas], int resultado[linhas]) {
    for (int j = 0; j < colunas; j++) {
        for (int i = 0; i < linhas; i++) {
            resultado[i] += matriz[i][j] * vetor[j];
        }
    }
}


int main() {
    struct timespec inicio, fim;
    double tempo_decorrido;

    const int n = 80000;

    const int m = n / 10;

    int matriz[n / 10][n];
    int vetor[n];
    
    int resutlado_row_major[n];
    int resultado_column_major[n];

    //inicializar matriz e vetor para teste
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            matriz[i][j] = i + j / 2;
        }
        vetor[i] = i + 5;
    }
    
    //inicializar resultado
    for(int i = 0; i < n; i++) {
        resutlado_row_major[i] = 0;
        resultado_column_major[i] = 0;   
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    multiplicacao_row_major(m, n, matriz, vetor, resultado_column_major);

    clock_gettime(CLOCK_MONOTONIC, &fim);


    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos (row major)\n", tempo_decorrido);

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    multiplicacao_column_major(m, n, matriz, vetor, resultado_column_major);

    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos (column major)\n", tempo_decorrido);

    return 0;
}
