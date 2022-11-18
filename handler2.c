#include "headers.h"

void handler2(int sig) {
    int pid, status;

    pid = waitpid(fg_pid, &status, WNOHANG);

    if(pid == 0) {
        
        if(kill(fg_pid, 19) < 0) { // SIGSTOP signal to the fg process
            perror("kill: Execution of command failed.\n");
            fflush(stderr);
            return;
        }

        // setting it in its own process id for background process
        setpgid(fg_pid, fg_pid);

        // tcsetpgrp(0, getpgid(0));

        push_node(&head, fg_pid, fg_name); // inserting it in the bg process queue
    }
}