/*
Tarefa 9:
Escreva um programa que cria tarefas
para realizar N inserções em duas listas
encadeadas, cada uma associada a uma
thread. Cada thread deve escolher
aleatoriamente em qual lista inserir um
número. Garanta a integridade das listas
evitando condição de corrida e, sempre
que possível, use regiões críticas
nomeadas para que a inserção em uma
lista não bloqueie a outra. Em seguida,
generalize o programa para um número
de listas definido pelo usuário. Explique
por que, nesse caso, regiões críticas
nomeadas não são suficientes e por que
o uso de locks explícitos se torna
necessário.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "linked_list.h"

#define N_TASKS 100
#define N_INSERCOES 5

void inserir_elementos_2_threads(Node** lista1, Node** lista2, int n) {
    #pragma omp parallel num_threads(2)
    {
        #pragma omp single
        {
            for (int t = 0; t < n; t++) {

                #pragma omp task
                {
                    int tid = omp_get_thread_num();
                    
                    for(int i = 0; i < N_INSERCOES; i++) {
                        int num = rand() % 1000;
                        int lista_escolhida = rand() % 2;

                        if (lista_escolhida == 0) {
                            #pragma omp critical(inserir_lista1)
                            {
                                add_node(lista1, num, tid);
                            }
                        } else {
                            #pragma omp critical(inserir_lista2)
                            {
                                add_node(lista2, num, tid);
                            }
                        }
                    }
                }
            }
        }
    }
}

void inserir_elementos_n_threads(Node** listas, omp_lock_t* locks, int num_listas_e_threads) {

    #pragma omp parallel num_threads(num_listas_e_threads)
    {
        #pragma omp single
        {
            for (int t = 0; t < N_TASKS; t++) {
                #pragma omp task
                {
                    int tid = omp_get_thread_num();
                    for(int i = 0; i < N_INSERCOES; i++) {
                        int num = rand() % 1000;
                        int lista_escolhida = rand() % num_listas_e_threads;

                        omp_set_lock(&locks[lista_escolhida]);
                        add_node(&listas[lista_escolhida], num, tid);
                        omp_unset_lock(&locks[lista_escolhida]);
                    }    
                }
            }
        }
    }
}



int main() {

    Node* lista1 = NULL;
    Node* lista2 = NULL;

    inserir_elementos_2_threads(&lista1, &lista2, N_TASKS);

    /*
    printf("Lista 1:\n");
    print_list(lista1);
    printf("Lista 2:\n");
    print_list(lista2);
    */

    int qtd_l1 = count_nodes(lista1);
    int qtd_l2 = count_nodes(lista2);
    printf("Contagems dos elementos de cada lista (Função com 2 listas): \n");
    printf("Lista 1: %d elemento%s\n", qtd_l1, qtd_l1 == 1 ? "" : "s");
    printf("Lista 2: %d elemento%s\n", qtd_l2, qtd_l2 == 1 ? "" : "s");

    free_list(lista1);
    free_list(lista2);
    
    // Segunda parte

    int num_lista_e_threads;

    printf("Digite o número de listas: ");
    scanf("%d", &num_lista_e_threads);

    Node** listas = malloc(num_lista_e_threads * sizeof(Node*));
    omp_lock_t* locks = malloc(num_lista_e_threads * sizeof(omp_lock_t));

    //inicializar listas e locks
    for (int i = 0; i < num_lista_e_threads; i++) {
        listas[i] = NULL;
        omp_init_lock(&locks[i]);
    }

    inserir_elementos_n_threads(listas, locks, num_lista_e_threads);
    /*
    for (int i = 0; i < num_lista_e_threads; i++) {
        printf("Lista %d:\n", i + 1);
        print_list(listas[i]);
    }*/

    printf("contagem de elementos de cada lista (Função com n listas):\n");
    for (int i = 0; i < num_lista_e_threads; i++) {
        int qtd = count_nodes(listas[i]);
        printf("Lista %d: %d elemento%s\n", i + 1, qtd, qtd == 1 ? "" : "s");
    }

    for (int i = 0; i < num_lista_e_threads; i++) {
        free_list(listas[i]);
        omp_destroy_lock(&locks[i]);
    }

    free(listas);
    free(locks);

    return 0;
}
