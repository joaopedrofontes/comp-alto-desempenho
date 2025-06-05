#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MAX_NAME_LEN 100

typedef struct Node {
    int data;
    int thread_id;  
    struct Node* next;
} Node;

Node* create_node(int data, int thread_id);
void add_node(Node** head, int data, int thread_id);
void print_list(Node* head);
void free_list(Node* head);
int count_nodes(Node* head);

#endif
