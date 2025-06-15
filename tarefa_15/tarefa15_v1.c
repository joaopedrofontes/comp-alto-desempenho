#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Parâmetros da simulação
#define TAMANHO_BARRA_GLOBAL 50000
#define PASSOS_TEMPO 10000
#define ALPHA 0.1
#define DT 0.1
#define DX 1.0

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            fprintf(stderr, "Este programa deve ser executado com 2 processos MPI.\n");
        }
        MPI_Finalize();
        return 1;
    }

    const int tamanho_local = TAMANHO_BARRA_GLOBAL / size;
    // Alocação estática na stack (+2 para células fantasmas)
    double u_local[tamanho_local + 2];
    double u_novo[tamanho_local + 2];

    // Condições iniciais
    for (int i = 0; i < tamanho_local + 2; i++) {
        u_local[i] = 0.0;
    }
    if (rank == 0) {
        u_local[1] = 100.0;
    }
    if (rank == 1) {
        u_local[tamanho_local] = 100.0;
    }
    
    // Sincroniza os processos antes de iniciar o cronômetro
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int t = 0; t < PASSOS_TEMPO; t++) {


        const int tag = 0;
        if (rank == 0) {

            MPI_Send(&u_local[tamanho_local], 1, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD);

            MPI_Recv(&u_local[tamanho_local + 1], 1, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else { // rank == 1

            MPI_Recv(&u_local[0], 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(&u_local[1], 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        }

        for (int i = 1; i <= tamanho_local; i++) {
            u_novo[i] = u_local[i] + ALPHA * DT / (DX * DX) *
                        (u_local[i + 1] - 2.0 * u_local[i] + u_local[i - 1]);
        }
        
        for (int i = 1; i <= tamanho_local; i++) {
             u_local[i] = u_novo[i];
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Versão Send/Recv Separado (Inseguro): Tempo total de execução: %f segundos\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}