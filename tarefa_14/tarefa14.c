/*
Tarefa 14:
Implemente um programa MPI com
exatamente dois processos. O processo 0
deve enviar uma mensagem ao processo
1, que imediatamente responde com a
mesma mensagem. Meça o tempo total
de execução de múltiplas trocas
consecutivas dessa mensagem,
utilizando MPI_Wtime. Registre os
tempos para diferentes tamanhos, desde
mensagens pequenas (como 8 bytes) até
mensagens maiores (como 1MB ou mais).
Analise graficamente o tempo em função
do tamanho da mensagem e identifique
os regimes onde a latência domina e
onde a largura de banda se torna o fator
principal.
*/



#include <mpi.h>
#include <stdio.h>
#include <string.h>


#define NUM_TROCAS 1000
#define TAM_MSG 65536

int main(int argc, char** argv) {
    int rank, size;
    char mensagem[TAM_MSG] = "Olá, processo!";
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Este programa precisa de exatamente 2 processos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    double inicio, fim;

    if (rank == 0) {
        
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int len;
        MPI_Get_processor_name(hostname, &len);
        printf("Hello from process %d on node %s!\n", rank, hostname);
        inicio = MPI_Wtime();
        

        for (int i = 0; i < NUM_TROCAS; i++) {
            MPI_Send(mensagem, TAM_MSG, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(mensagem, TAM_MSG, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        }

        fim = MPI_Wtime();
        printf("Tempo total para %d trocas: %f segundos\n", NUM_TROCAS, fim - inicio);
    } else if (rank == 1) {

        char hostname[MPI_MAX_PROCESSOR_NAME];
        int len;
        MPI_Get_processor_name(hostname, &len);

        printf("Hello from process %d on node %s!\n", rank, hostname);
        for (int i = 0; i < NUM_TROCAS; i++) {
            MPI_Recv(mensagem, TAM_MSG, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(mensagem, TAM_MSG, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
