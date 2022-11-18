#include "headers.h"

void handler1(int sig) {
    int pid, status;

    struct Node *p = head, *temp;

    while(p != NULL) {

        pid = waitpid(p->pid, &status, WNOHANG);
        
        if(pid != 0) {

            fprintf(stderr, "%s with pid %d exited", p->name, p->pid);
            fflush(stderr);

            if(WIFEXITED(status)) {
                fprintf(stderr, " normally.\n");
            }
            else {
                fprintf(stderr, " abnormally.\n");
            }

            fflush(stderr);
            
            temp = p->next;
            delete_node(&head, p->pid);
            p = temp;
        }
        else {
            p = p->next;
        }
    }
}