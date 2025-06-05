/*
Tarefa 2:
Implemente três laços em C para
investigar os efeitos do paralelismo ao
nível de instrução (ILP): 1) inicialize um
vetor com um cálculo simples; 2) some
seus elementos de forma acumulativa,
criando dependência entre as iterações;
e 3) quebre essa dependência utilizando
múltiplas variáveis. Compare o tempo de
execução das versões compiladas com
diferentes níveis de otimização (O0, O2,
O3) e analise como o estilo do código e
as dependências influenciam o
desempenho.
*/



#include <stdio.h>
#include <time.h>


int soma_comum(int n, int vetor[n]) {
    int soma = 0;
    for(int i = 0; i < n; i++) {
        soma += vetor[i];
    }
    return soma;
}

int soma_ilp(int n, int vetor[n]) {
    int soma_1 = 0;
    int soma_2 = 0;
    for(int i = 0; i < n / 2; i += 2) {
        soma_1 += vetor[i];
        soma_2 += vetor[i + 1];
    }

    return soma_1 + soma_2;
}


int main() {

    struct timespec inicio, fim;
    double tempo_decorrido;

    //limite: 1 000 000 000     
    int n = 1000000000;

    int vetor[n];

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    for(int i = 0; i < n; i++) {
        vetor[i] = i + 3;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos (Atribuição)\n", tempo_decorrido);
    
    //primeiro teste (soma comum)
    int soma = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    soma = soma_comum(n, vetor);
    
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos (Soma comum)\n", tempo_decorrido);

    //segundo teste (soma ilp)
    int soma_ilp_res = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    soma_ilp_res = soma_ilp(n, vetor);
    
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos (Soma ilp)\n", tempo_decorrido);

    return 0;
}