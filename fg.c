#include "headers.h"

void fg(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "fg: Syntax error, fg command takes 1 argument.\n");
        return;
    }

    errno = 0;

    char *str, *endptr;
    long val;

    str = argv[1];
    val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0)) {
        perror("fg: Invalid arguments, either not integer or overflow.");
        return;
    } 

    if (endptr == str || *endptr != '\0') {
        fprintf(stderr, "fg: Characters other than numbers found.\n");
        return;
    }

    if(val <= 0) {
        fprintf(stderr, "fg: Invalid job no., since input is <= 0.\n");
        return;
    }

// ----------------------------------------------------------------------------------

    int job_pid = 0;
    int pid, status, job_no = 0;
    char* job_name = (char*) calloc(const_size, sizeof(char));

    struct Node* p = head;

    while(p != NULL) {

        pid = waitpid(p->pid, &status, WNOHANG);
        
        if(pid == 0) { 
            job_no++;

            if(job_no == val) {
                job_pid = p->pid;
                strcpy(job_name, p->name);
                break;
            }
        }
        
        p = p->next;
    }

    if(p == NULL) {
        fprintf(stderr, "fg: Invalid Job Number.\n");
        return;
    }

// ----------------------------------------------------------------------------------

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, handler2);
    signal(SIGINT, handler3);

    
    // Assign it to foreground process group id
    setpgid(job_pid, getpgid(0));
    

    // tcsetpgrp(0, job_pid); 

    // continue the process from stopped state
    if(kill(job_pid, 18) < 0) {
        perror("fg: Execution of command failed.");
        return;
    }

    delete_node(&head, job_pid); // delete from bg processes queue

    fg_pid = job_pid;
    strcpy(fg_name, job_name);

    waitpid(job_pid, &status, WUNTRACED); // wait

    // tcsetpgrp(0, getpgid(0)); // reassign the fg process


    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_DFL);
}   
