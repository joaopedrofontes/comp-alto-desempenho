
/*
Tarefa 10:
Implemente novamente o estimador da
tarefa 8 que usa um contador
compartilhado e o rand_r substituindo
o #pragma omp critical pelo
#pragma omp atomic. Compare
essas duas implementações com suas
versões que usam contadores privados.
Agora, compare essas com uma 5ª
versão que utiliza apenas a cláusula
reduction ao invés das diretivas de
sincronização. Reflita sobre a
aplicabilidade de desses mecanismos em
termos de desempenho e produtividade e
proponha um roteiro para quando utilizar
qual mecanismo de sincronização,
incluindo critical nomeadas e
locks
explícitos.
*/


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double estimativa_pi_shared_critical(int n) {
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

double estimativa_pi_shared_atomic(int n) {
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
                #pragma omp atomic
                contador++;
            }
        }
    }

    return 4.0 * contador / n;
}



double estimativa_pi_private_critical(int n) {
    int contador_total = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();
        int contador_local = 0;

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;

            if (dx * dx + dy * dy <= 0.25) {
                contador_local++;
            }
        }

        #pragma omp critical
        {
            contador_total += contador_local;
        }
    }

    return 4.0 * contador_total / n;
}


double estimativa_pi_private_atomic(int n) {
    int contador_total = 0;

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();
        int local_contador = 0; 

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;
            if (dx * dx + dy * dy <= 0.25) {
                local_contador++;
            }
        }

        #pragma omp atomic
        contador_total += local_contador;  
    }

    return 4.0 * contador_total / n;
}


double estimativa_pi_reduction(int n) {
    int contador = 0;

    #pragma omp parallel reduction(+:contador)
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;

            double dx = x - 0.5;
            double dy = y - 0.5;

            if (dx * dx + dy * dy <= 0.25) {
                contador++;
            }
        }
    }

    return 4.0 * contador / n;
}

int main() {
    srand(time(NULL));
    double inicio, fim;
    int n = 1000000000;

    inicio = omp_get_wtime();
    double pi_1 = estimativa_pi_shared_critical(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função com critical: %f\n", pi_1);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    inicio = omp_get_wtime();
    double pi_2 = estimativa_pi_shared_atomic(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função com atomic: %f\n", pi_2);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    
    inicio = omp_get_wtime();
    double pi_3 = estimativa_pi_private_critical(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função com var privadas e critical para somar: %f\n", pi_3);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");

    
    inicio = omp_get_wtime();
    double pi_4 = estimativa_pi_private_atomic(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função com var privadas e atomic para somar: %f\n", pi_4);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");
    

    inicio = omp_get_wtime();
    double pi_5 = estimativa_pi_reduction(n);
    fim = omp_get_wtime();
    printf("Estimativa de pi da função com reduction: %f\n", pi_4);
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("================                       ========================\n");
    
    return 0;
}
