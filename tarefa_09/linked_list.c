#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

Node* create_node(int data, int thread_id) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->thread_id = thread_id;
    new_node->next = NULL;
    return new_node;
}

void add_node(Node** head, int data, int thread_id) {
    Node* new_node = create_node(data, thread_id);
    new_node->next = *head;
    *head = new_node;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("Data: %d, Thread ID: %d\n", current->data, current->thread_id);
        current = current->next;
    }
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

int count_nodes(Node* head) {
    Node* current = head;
    int counter = 0;
    while (current != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}
