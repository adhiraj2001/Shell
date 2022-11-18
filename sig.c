#include "headers.h"

void sig(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "sig: Syntax error, sig command takes 2 argument.\n");
        return;
    }

    errno = 0;

    char *str1, *endptr1;
    long val1;

    str1 = argv[1];
    val1 = strtol(str1, &endptr1, 10);

    if ((errno == ERANGE && (val1 == LONG_MAX || val1 == LONG_MIN)) || (errno != 0)) {
        perror("sig: Invalid arguments, either not integer or overflow.");
        return;
    } 

    if (endptr1 == str1 || *endptr1 != '\0') {
        fprintf(stderr, "sig: Characters other than numbers found.\n");
        return;
    }

    if(val1 <= 0) {
        fprintf(stderr, "sig: Invalid job no., since input is <= 0.\n");
        return;
    }


    char *str2, *endptr2;
    long val2;

    str2 = argv[2];
    val2 = strtol(str2, &endptr2, 10);

    if ((errno == ERANGE && (val2 == LONG_MAX || val2 == LONG_MIN)) || (errno != 0)) {
        perror("sig: Invalid arguments, either not integer or overflow.");
        return;
    } 

    if (endptr2 == str2 || *endptr2 != '\0') {
        fprintf(stderr, "sig: Characters other than numbers found.\n");
        return;
    }

    if(val2 < 0 || val2 > 31) {
        fprintf(stderr, "sig: Invalid signal no., since input is < 0 or > 31.\n");
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

            if(job_no == val1) {
                job_pid = p->pid;
                strcpy(job_name, p->name);
                break;
            }
        }
        
        p = p->next;
    }

    if(p == NULL) {
        fprintf(stderr, "sig: Invalid Job Number.\n");
        return;
    }

// ----------------------------------------------------------------------------------

    if(kill(job_pid, val2) < 0) {
        perror("sig: Execution of kill command failed.");
        return;
    }
}
