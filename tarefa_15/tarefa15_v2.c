#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
        if (rank == 0) fprintf(stderr, "Execute com 2 processos MPI.\n");
        MPI_Finalize();
        return 1;
    }

    const int tamanho_local = TAMANHO_BARRA_GLOBAL / size;
    double u_local[tamanho_local + 2];
    double u_novo[tamanho_local + 2];
    
    // Condições iniciais
    for (int i = 0; i < tamanho_local + 2; i++) u_local[i] = 0.0;
    if (rank == 0) u_local[1] = 100.0;
    if (rank == 1) u_local[tamanho_local] = 100.0;

    int vizinho_esq = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int vizinho_dir = (rank == 1) ? MPI_PROC_NULL : rank + 1;

    MPI_Request requests[4];
    MPI_Status statuses[4];

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int t = 0; t < PASSOS_TEMPO; t++) {
        // 1. Inicia a comunicação não-bloqueante
        MPI_Irecv(&u_local[0], 1, MPI_DOUBLE, vizinho_esq, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&u_local[tamanho_local + 1], 1, MPI_DOUBLE, vizinho_dir, 1, MPI_COMM_WORLD, &requests[1]);
        MPI_Isend(&u_local[1], 1, MPI_DOUBLE, vizinho_esq, 1, MPI_COMM_WORLD, &requests[2]);
        MPI_Isend(&u_local[tamanho_local], 1, MPI_DOUBLE, vizinho_dir, 0, MPI_COMM_WORLD, &requests[3]);

        // 2. Calcula os pontos internos (que não dependem dos dados da comunicação)
        for (int i = 2; i < tamanho_local; i++) {
            u_novo[i] = u_local[i] + ALPHA * DT / (DX * DX) *
                        (u_local[i + 1] - 2.0 * u_local[i] + u_local[i - 1]);
        }

        // 3. Espera a conclusão da comunicação
        MPI_Waitall(4, requests, statuses);
        
        // 4. Calcula as bordas (agora que os dados fantasmas foram recebidos)
        u_novo[1] = u_local[1] + ALPHA * DT / (DX * DX) * (u_local[2] - 2.0 * u_local[1] + u_local[0]);
        u_novo[tamanho_local] = u_local[tamanho_local] + ALPHA * DT / (DX * DX) * (u_local[tamanho_local+1] - 2.0*u_local[tamanho_local] + u_local[tamanho_local-1]);

        // 5. Copia todos os valores calculados
        for (int i = 1; i <= tamanho_local; i++) {
            u_local[i] = u_novo[i];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Versão Não-Bloqueante (Wait): Tempo total de execução: %f segundos\n", end_time - start_time);
    }
    
    MPI_Finalize();
    return 0;
}