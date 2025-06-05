/*
Tarefa 7:
Implemente um programa em C que cria
uma lista encadeada com nós, cada um,
contendo o nome de um arquivo fictício.
Dentro de uma região paralela, percorra
a lista e crie uma tarefa com #pragma
omp task para processar cada nó.
Cada tarefa deve imprimir o nome do
arquivo e o identificador da thread que a
executou. Após executar o programa,
reflita: todos os nós foram processados?
Algum foi processado mais de uma vez
ou ignorado? O comportamento muda
entre execuções? Como garantir que
cada nó seja processado uma única vez
e por apenas uma tarefa?
*/


#include <stdio.h>
#include "linked_list.h"

int main() {
    Node* head = NULL;

    const char* jogadores[] = { 
        "Cafu", "Denílson", 
        "Gilberto Silva", 
        "Kléberson", "Lúcio",
        "Marcos", "Roberto Carlos", 
        "Roque Júnior", "Ronaldo",
        "Ronaldinho", "Rivaldo"
    };

    int n = sizeof(jogadores) / sizeof(jogadores[0]);
    for (int i = 0; i < n; i++) {
        add_node(&head, jogadores[i]);
    }

    printf("Lista dos jogadores pentacampeões do Brasil (2002):\n");
    print_list_single(head);

    free_list(head);
    return 0;
}
