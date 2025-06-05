/*
Tarefa 12:
Avalie a escalabilidade do seu código
de Navier-Stokes utilizando algum nó de
computação no NPAD. Procure
identificar gargalos de escalabilidade e
reporte o seu progresso em versões
sucessivas da evolução do código
otimizado. Comente sobre a
escalabilidade, a escalabilidade fraca e a
escalabilidade fortes das versões.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>



// Parâmetros da simulação (aumentados para 3D completo)
#define NX 320       // Número de pontos em x 
#define NY 320       // Número de pontos em y
#define NZ 320       // Número de pontos em z
#define NT 100      // Número de passos de tempo
#define DX 0.1f     // Tamanho do passo espacial 
#define DY 0.1f    
#define DZ 0.1f    
#define DT 0.0005f  // Passo temporal 
#define NU 1.0f     // Coeficiente de viscosidade

// Estrutura para campo de velocidade 3D 
typedef struct {
    float u[NX][NY][NZ]; // Componente x
    float v[NX][NY][NZ]; // Componente y
    float w[NX][NY][NZ]; // Componente z
} VelocityField3D;

// Cabeçalho para o arquivo binário
typedef struct {
    int nx, ny, nz;      // Dimensões da malha
    int nt;              // Número total de passos de tempo
    float dx, dy, dz;    // Espaçamento espacial
    float dt;            // Passo temporal
    float nu;            // Viscosidade
    int time_step;       // Passo de tempo atual
} BinHeader;

// Inicialização do campo 3D
void initialize_3d(VelocityField3D *vel, int initial_condition) {
    memset(vel, 0, sizeof(VelocityField3D)); // Zera todo o campo

    if (initial_condition == 1) {
        // Preenche com velocidade constante
        for (int i = 0; i < NX; i++) {
            for (int j = 0; j < NY; j++) {
                for (int k = 0; k < NZ; k++) {
                    vel->u[i][j][k] = 1.0f;
                    vel->v[i][j][k] = 1.0f;
                    vel->w[i][j][k] = 1.0f;
                }
            }
        }
    }

    const int center_x = NX/2, center_y = NY/2, center_z = NZ/2;
    const int radius = 5;
    const float amplitude = 0.5f;
    
    for (int i = center_x - radius; i <= center_x + radius; i++) {
        for (int j = center_y - radius; j <= center_y + radius; j++) {
            for (int k = center_z - radius; k <= center_z + radius; k++) {
                if (i >= 0 && i < NX && j >= 0 && j < NY && k >= 0 && k < NZ) {
                    float dist = sqrtf(powf(i-center_x,2) + powf(j-center_y,2) + powf(k-center_z,2));
                    if (dist <= radius) {
                        float perturbation = amplitude * expf(-dist);
                        vel->u[i][j][k] += perturbation;
                        vel->v[i][j][k] += perturbation;
                        vel->w[i][j][k] += perturbation;
                    }
                }
            }
        }
    }
}

// Condições de contorno 3D (paredes fixas)
void apply_boundary_conditions_3d(VelocityField3D *vel) {
    // Faces x=0 e x=NX-1
    for (int j = 0; j < NY; j++) {
        for (int k = 0; k < NZ; k++) {
            vel->u[0][j][k] = vel->v[0][j][k] = vel->w[0][j][k] = 0.0f;
            vel->u[NX-1][j][k] = vel->v[NX-1][j][k] = vel->w[NX-1][j][k] = 0.0f;
        }
    }
    
    // Faces y=0 e y=NY-1
    for (int i = 0; i < NX; i++) {
        for (int k = 0; k < NZ; k++) {
            vel->u[i][0][k] = vel->v[i][0][k] = vel->w[i][0][k] = 0.0f;
            vel->u[i][NY-1][k] = vel->v[i][NY-1][k] = vel->w[i][NY-1][k] = 0.0f;
        }
    }
    
    // Faces z=0 e z=NZ-1
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < NY; j++) {
            vel->u[i][j][0] = vel->v[i][j][0] = vel->w[i][j][0] = 0.0f;
            vel->u[i][j][NZ-1] = vel->v[i][j][NZ-1] = vel->w[i][j][NZ-1] = 0.0f;
        }
    }
}

void update_velocity_3d(const VelocityField3D *vel, VelocityField3D *new_vel) {
    const float dx2 = DX*DX, dy2 = DY*DY, dz2 = DZ*DZ;
    const float factor = DT * NU;

    #pragma omp parallel 
    {    
        //#pragma omp for schedule(static)
        //#pragma omp for collapse(3) schedule(static)
        //#pragma omp for schedule(dynamic)
        //#pragma omp for collapse(3) schedule(dynamic, 8)
        //#pragma omp for schedule(guided)
        #pragma omp for collapse(3) schedule(guided)
        for (int i = 1; i < NX-1; i++) {
            for (int j = 1; j < NY-1; j++) {
                for (int k = 1; k < NZ-1; k++) {

                    float laplacian_u = 
                        (vel->u[i+1][j][k] - 2*vel->u[i][j][k] + vel->u[i-1][j][k])/dx2 +
                        (vel->u[i][j+1][k] - 2*vel->u[i][j][k] + vel->u[i][j-1][k])/dy2 +
                        (vel->u[i][j][k+1] - 2*vel->u[i][j][k] + vel->u[i][j][k-1])/dz2;
                    
                    float laplacian_v = 
                        (vel->v[i+1][j][k] - 2*vel->v[i][j][k] + vel->v[i-1][j][k])/dx2 +
                        (vel->v[i][j+1][k] - 2*vel->v[i][j][k] + vel->v[i][j-1][k])/dy2 +
                        (vel->v[i][j][k+1] - 2*vel->v[i][j][k] + vel->v[i][j][k-1])/dz2;
                    
                    float laplacian_w = 
                        (vel->w[i+1][j][k] - 2*vel->w[i][j][k] + vel->w[i-1][j][k])/dx2 +
                        (vel->w[i][j+1][k] - 2*vel->w[i][j][k] + vel->w[i][j-1][k])/dy2 +
                        (vel->w[i][j][k+1] - 2*vel->w[i][j][k] + vel->w[i][j][k-1])/dz2;
                    

                    new_vel->u[i][j][k] = vel->u[i][j][k] + factor * laplacian_u;
                    new_vel->v[i][j][k] = vel->v[i][j][k] + factor * laplacian_v;
                    new_vel->w[i][j][k] = vel->w[i][j][k] + factor * laplacian_w;
                }
            }
        }
    }
}

/*
void save_full_3d_binary(const VelocityField3D *vel, int time_step, FILE *outfile) {
    BinHeader header = {
        .nx = NX, .ny = NY, .nz = NZ,
        .nt = NT,
        .dx = DX, .dy = DY, .dz = DZ,
        .dt = DT,
        .nu = NU,
        .time_step = time_step
    };

    fwrite(&header, sizeof(BinHeader), 1, outfile);
    
    fwrite(vel->u, sizeof(float), NX*NY*NZ, outfile);
    fwrite(vel->v, sizeof(float), NX*NY*NZ, outfile);
    fwrite(vel->w, sizeof(float), NX*NY*NZ, outfile);
}
*/
int main() {
    VelocityField3D vel, new_vel;
    int initial_condition = 0; 
    
    double incio, fim;

    /*
    FILE *outfile = fopen("fluid3d_simulation.bin", "wb");
    if (!outfile) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }
    */
    
    int n_threads;

    #pragma omp parallel
    {
        #pragma omp single
        {
            n_threads = omp_get_num_threads();
        }
    }

    incio = omp_get_wtime();

    initialize_3d(&vel, initial_condition);
    //save_full_3d_binary(&vel, 0, outfile); 
    for (int t = 1; t <= NT; t++) {
        update_velocity_3d(&vel, &new_vel);
        apply_boundary_conditions_3d(&new_vel);
        
    
        memcpy(&vel, &new_vel, sizeof(VelocityField3D));
        
        if (t % 10 == 0) {
            //save_full_3d_binary(&vel, t, outfile); 
        }
    }

    fim = omp_get_wtime();  
    
    printf("Numero de threads: %d\n", n_threads);
    
    printf("Tempo total da simulação: %.3f segundos\n", fim - incio);
    
    //fclose(outfile);

    //salvamentos estão comentados
    printf("Simulação completa.\n");
    //printf("Dados salvos em fluid3d_simulation.bin\n");
    
    return 0;
}