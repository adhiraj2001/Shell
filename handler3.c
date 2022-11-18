#include "headers.h"

void handler3(int sig) {
    int pid, status;

    pid = waitpid(fg_pid, &status, WNOHANG);

    if(pid == 0) {

        if(kill(fg_pid, 9) < 0) { // SIGKILL signal to terminate 
            perror("kill: Execution of command failed.\n");
            fflush(stderr);
            return;
        }
    }
}