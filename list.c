#include "headers.h"

void push_node(struct Node** head, int pid, char name[]) {

    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  
    new_node->pid  = pid;
    strcpy(new_node->name, name);
    new_node->next = NULL;

    if(*head == NULL) {
        *head = new_node;
        return;
    }

    struct Node* last = *head;

    while(last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
    return;
}

void delete_node(struct Node** head, int pid) {
    struct Node *temp = *head, *prev;
 
    if (temp != NULL && temp->pid == pid) {
        *head = temp->next;
        free(temp);
        return;
    }
 
    while (temp != NULL && temp->pid != pid) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return;
    }

    prev->next = temp->next;
 
    free(temp);
    return;
}