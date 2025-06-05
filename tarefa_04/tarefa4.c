
/*
Tarefa 4:
Implemente dois programas paralelos em
C com OpenMP: um limitado por
memória, com somas simples em vetores,
e outro limitado por CPU, com cálculos
matemáticos intensivos. Paralelize com
#pragma omp parallel for e
meça o tempo de execução variando o
número de threads. Analise quando o
desempenho melhora, estabiliza ou piora,
e reflita sobre como o multithreading de
hardware pode ajudar em programas
memory-bound, mas atrapalhar em
programas compute-bound pela
competição por recursos.
*/

#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>


void func_memory_bound(int tam_vetor, int vetor_1[tam_vetor], int vetor_2[tam_vetor], int vetor_3[tam_vetor], int vetor_4[tam_vetor]) {
    #pragma omp parallel for num_threads(10)
    for(int i = 0; i < tam_vetor; i++) {
        vetor_4[i] = vetor_1[i] + vetor_2[i] - vetor_3[i];
    }
}

void func_cpu_bound() {
    double x = 0.004;

    #pragma omp parallel for num_threads(20)
    for (int i = 1; i < 10000000; i++) {
        x += sin(x) + cos(x) + tan(x) + sin(x) + cos(x) - tan(x - 00.3) +
         sin(x) + cos(x) + tan(x) + sin(x) + cos(x) - tan(x - 00.3)
        + sin(x) + cos(x) + tan(x) + sin(x) + cos(x) - tan(x - 00.3);
    }
}

int main() {
    double inicio;
    double fim;

    const int tam_vetores = 1000000000;
    /*
    int vetor_1[tam_vetores];
    int vetor_2[tam_vetores];
    int vetor_3[tam_vetores];
    int vetor_4[tam_vetores];
    //iniciar vetores
    for(int i = 0; i < tam_vetores; i++) {
        vetor_1[i] = i + 1;
        vetor_2[i] = i + 2;
        vetor_3[i] = i + 3;
    }

    */

    /*
    printf("\n========MEMORY BOUND========\n");

    inicio = omp_get_wtime();
    func_memory_bound(tam_vetores, vetor_1, vetor_2, vetor_3, vetor_4);
    fim = omp_get_wtime();
    
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    */

    
    printf("\n========CPU BOUND========\n");

    inicio = omp_get_wtime();
    func_cpu_bound();
    fim = omp_get_wtime();
    
    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
}

