
/*
Tarefa 8:
Implemente estimativa estocástica de π
usando rand() para gerar os pontos.
Cada thread deve usar uma variável
privada para contar os acertos e
acumular o total em uma variável global
com #pragma omp critical.
Depois, implemente uma segunda versão
em que cada thread escreve seus
acertos em uma posição distinta de um
vetor compartilhado. A acumulação deve
ser feita em um laço serial após a região
paralela. Compare o tempo de execução
das duas versões. Em seguida, substitua
rand() por rand_r() em ambas e
compare novamente. Explique o
comportamento dos quatro programas
com base na coerência de cache e nos
efeitos do falso compartilhamento.
*/



#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


double estimativa_pi(int n) {
    int contador = 0;
    double x, y;
    for (int i = 0; i < n; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        double dx = x - 0.5;
        double dy = y - 0.5;
        if (dx * dx + dy * dy <= 0.25) {

            contador++; 
        }
    }

    return 4.0 * contador / n;
}

double estimativa_pi_2(int n) {
    int acertos[8] = {0};  
    int total = 0;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;

            if (dx * dx + dy * dy <= 0.25) {
                acertos[tid]++;
            }
        }

        #pragma omp single
        {
            for (int i = 0; i < 8; i++) {
                total += acertos[i];
            }
        }
    }

    return 4.0 * total / n;
}

double estimativa_pi_3(int n) {
    int contador = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;
            if (dx * dx + dy * dy <= 0.25) {
                #pragma omp critical
                contador++;
            }
        }
    }

    return 4.0 * contador / n;
}

double estimativa_pi_4(int n) {
    int acertos[8] = {0};
    int total = 0;

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        unsigned int seed = time(NULL) + tid;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;

            if (dx * dx + dy * dy <= 0.25) {
                acertos[tid]++;
            }
        }

        #pragma omp single
        {
            for (int i = 0; i < 8; i++) {
                total += acertos[i];
            }
        }
    }

    return 4.0 * total / n;
}

int main() {
    srand(time(NULL));
    double inicio, fim;
    int n = 100000000;

    inicio = omp_get_wtime();
    double pi_1 = estimativa_pi_1(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função 1: %f\n", pi_1);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    inicio = omp_get_wtime();
    double pi_2 = estimativa_pi_2(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função 2: %f\n", pi_2);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    inicio = omp_get_wtime();
    double pi_3 = estimativa_pi_3(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função 3: %f\n", pi_3);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    inicio = omp_get_wtime();
    double pi_4 = estimativa_pi_4(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função 4: %f\n", pi_4);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    return 0;
}
