#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "linked_list.h"

Node* create_node(const char* name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Erro de alocação!\n");
        exit(1);
    }
    strcpy(new_node->data, name);
    new_node->next = NULL;
    return new_node;
}

void add_node(Node** head, const char* name) {
    Node* new_node = create_node(name);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }
}


void print_list(Node* head) {
    #pragma omp parallel
    {
        Node* current = head;
        while (current != NULL) {
            #pragma omp task
            {
                printf("Jogador: %s | Thread: %d\n", current->data, omp_get_thread_num());
            }
            current = current->next;
        }
    }
}

void print_list_single(Node* head) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            Node* current = head;
            while (current != NULL) {
                #pragma omp task
                {
                    printf("Jogador: %s | Thread: %d\n", current->data, omp_get_thread_num());
                }
                current = current->next;
            }
        }
    }
}



void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}
