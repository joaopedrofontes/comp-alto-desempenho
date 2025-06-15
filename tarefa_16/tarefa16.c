#include <mpi.h>
#include <stdio.h>

#define M 10000  
#define N 2000
int main(int argc, char** argv) {
    int rank, size;
    float A[M][N], x[N], y[M]; 
    float start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (M % size != 0) {
        if (rank == 0)
            printf("Erro: M (%d) não é divisível pelo número de processos (%d).\n", M, size);
        MPI_Finalize();
        return 1;
    }

    int linhas_por_proc = M / size;
    float subA[linhas_por_proc][N];  /
    float subY[linhas_por_proc];    

    if (rank == 0) {
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                A[i][j] = i + j;

        for (int i = 0; i < N; i++)
            x[i] = 1.0;
    }

    if (rank == 0)
        start_time = MPI_Wtime();

    MPI_Scatter(A, linhas_por_proc * N, MPI_FLOAT,
                subA, linhas_por_proc * N, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    MPI_Bcast(x, N, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < linhas_por_proc; i++) {
        subY[i] = 0.0;
        for (int j = 0; j < N; j++)
            subY[i] += subA[i][j] * x[j];
    }

    MPI_Gather(subY, linhas_por_proc, MPI_FLOAT,
               y, linhas_por_proc, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Tempo de execução: %f segundos\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}