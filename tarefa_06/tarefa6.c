/*
Tarefa 6:
Implemente em C a estimativa
estocástica de π. Paralelize com
#pragma omp parallel for e
explique o resultado incorreto. Corrija a
condição de corrida utilizando o
#pragma omp critical e
reestruturando com #pragma omp
parallel seguido de #pragma omp
for e aplicando as cláusulas private,
firstprivate, lastprivate e
shared. Teste diferentes combinações
e explique como cada cláusula afeta o
comportamento do programa. Comente
também como a cláusula
default(none) pode ajudar a tornar
o escopo mais claro em programas
complexos.
*/




#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


double estimativa_pi_1(int n) {
    int contador = 0;
    double x, y;

    #pragma omp parallel
    {  
        #pragma omp for 
        for (int i = 0; i < n; i++) {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;

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

double estimativa_pi(int n) {
    int num_threads;
    int *acertos;

    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    acertos = (int *)calloc(num_threads, sizeof(int));

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        double x, y;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;
            if (dx * dx + dy * dy <= 0.25) {
                acertos[id]++;
            }
        }
    }

    int total_acertos = 0;
    for (int i = 0; i < num_threads; i++) {
        total_acertos += acertos[i];
    }

    free(acertos);
    return 4.0 * total_acertos / n;
}

int main() {
    srand(time(NULL)); 

    int n = 10000000; 
    double pi = estimativa_pi_1(n);

    printf("Estimativa de pi da função 1: %f\n", pi);
    return 0;
}

