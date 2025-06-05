/*
Tarefa 5:
Implemente um programa em C que conte
quantos números primos existem entre 2
e um valor máximo
n. depois, paralelize o
laço principal usando a diretiva
#pragma omp parallel for sem
alterar a lógica original. compare o
tempo de execução e os resultados das
versões sequencial e paralela. observe
possíveis diferenças no resultado e no
desempenho, e reflita sobre os desafios
iniciais da programação paralela, como
correção e distribuição de carga.
*/



#include <stdio.h>
#include <omp.h>
#include <sys/time.h>


int contar_primos(int n) {
    int contador = 0;

    #pragma omp parallel for num_threads(8)
    for (int i = 2; i <= n; i++) {
        int primo = 1;

        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                primo = 0;
                break;
            }
        }

        if (primo) {
            contador++;
        }
    }

    return contador;
}

int main() {

    double inicio;
    double fim;

    const int n = 100000000;

    inicio = omp_get_wtime();
    int quantidade = contar_primos(n);
    fim = omp_get_wtime();

    printf("Tempo decorrido: %.6f segundos\n", fim - inicio);
    printf("Quantidade de primos contados: %d\n", quantidade);
    return 0;
}
