#ifndef __LIST_H
#define __LIST_H

struct Node {
    int pid;
    char name[256];

    struct Node *next;
};

void push_node(struct Node** head, int pid, char* name);
void delete_node(struct Node** head, int pid);

#endif

