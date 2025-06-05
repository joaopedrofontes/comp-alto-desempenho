#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MAX_NAME_LEN 100

typedef struct Node {
    char data[MAX_NAME_LEN];
    struct Node* next;
} Node;

Node* create_node(const char* name);
void add_node(Node** head, const char* name);
void print_list(Node* head);
void print_list_single(Node* head);
void free_list(Node* head);

#endif
