#include "headers.h"

void execute(int argc, char **argv, char* input_file, char* output_file, int append) {
    int pid, status;
    int flag_bg = 0;

    if(strcmp(argv[argc-1], "&") == 0) {
        free(argv[argc]);
        argv[argc-1] = NULL;
        argc--;

        flag_bg = 1;
    }

    for(int i=0; i<argc; i++) {
        if(argv[i][0] == '~') {
            char* temp = (char*) calloc(const_size, sizeof(char));
            sprintf(temp, "%s/%s", root_dir, argv[i] + 1);

            argv[i] = temp;
        }
    }

// ---------------------------------------------------------------------------------------


    pid = fork();
    
    if (pid < 0) {
        perror("Error: Forking child process failed");
        return;
    }
    else if (pid == 0) {
        
        // signal handling for child process
        signal(SIGTSTP, SIG_DFL);
        signal(SIGINT, SIG_DFL);

        // Give bg child process its own process group
        if(flag_bg) {
            setpgid(0, 0);
        }

// -------------------------------------------------------------------------------------------

        // flushing input and output buffers
        fflush(stdin);
        fflush(stdout);

        if(strlen(input_file) > 0) {

            int fd = open(input_file, O_RDONLY, 0644);
            
            if(fd < 0) {
                perror("fd1: File failed to open input-file.");
                exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(strlen(output_file) > 0) {

            int fd = -1;

            if(append) {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else {
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }

            if(fd < 0) {
                perror("fd2: Failed to create or open output-file.");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

// ----------------------------------------------------------------------------------------------

        // Executing execvp, this will overwrite rest of the program if execution is successful.
        int k = execvp(argv[0], argv);

        if (k < 0) {
            perror("Error: Command doesn't exist or execution failed.");
            exit(1);
        }

        exit(0);
    }
    else {
        if(flag_bg) {
            printf("%d\n", pid);
            push_node(&head, pid, argv[0]); // push pid and name of background process in the linked list.
        }
        else {
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTSTP, handler2);
            signal(SIGINT, handler3);

            fg_pid = pid;
            strcpy(fg_name, argv[0]);

            // Assign foreground process group to the child process group
            // tcsetpgrp(0, pid);
            

            // wait for execution of foreground process.
            waitpid(pid, &status, WUNTRACED);


            // Assign the foreground process group back to the parent process group
            // tcsetpgrp(0, getpgid(0));

            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_IGN);
            signal(SIGTTOU, SIG_DFL);
        }
    }
}