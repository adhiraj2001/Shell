#include "headers.h"

void piped_command(char* command_line) {

    int n = strlen(command_line);
    for(int i=0; i < n; i++) {
        if(command_line[i] == '|' && (i == 0 || i == n - 1 || command_line[i-1] == '|')) {
            fprintf(stderr, "Syntax error: No command found to be piped with '|'.\n");
            return;
        }
    }

    char** commands = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        commands[i] = (char*) calloc(const_size, sizeof(char));
    }

    char** args = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        args[i] = (char*) calloc(const_size, sizeof(char));
    }


    int prev_input = STDIN_FILENO; // stores input pipe end for previous command
    int fd[2]; // stores fd for file
    int pid, status;

// ----------------------------------------------------------------------------------------

    int n1 = parse(command_line, commands, "|");

    if(n1 == 0) {
        fprintf(stderr, "Syntax error: No command found to be piped with '|'.\n");
        return;
    }

    for(int i=0; i < n1; i++) {

        char delimit[] = " \t\r\n\v\f";

        int n2 = parse(commands[i], args, delimit);
            
        if(n2 == 0) {
            fprintf(stderr, "Syntax error: No command to be piped.\n");
            return;
        }

        if(strcmp(args[0], "exit") == 0) {
            exit(0);
        }

// -----------------------------------------------------------------------------------------

        char* input_file = (char*) calloc(const_size, sizeof(char));
        char* output_file = (char*) calloc(const_size, sizeof(char));

        int append = 0;
        
        int flag_error = redirect(&n2, args, input_file, output_file, &append);

        if(flag_error) {
            fprintf(stderr, "><: Incorrect syntax for input - output redirection.\n");
            return;
        }

        if(n2 == 0) {
            fprintf(stderr, "Syntax error: No command found to be piped.\n");
            return;
        }

// ------------------------------------------------------------------------------------------

        // bg handling
        int flag_bg = 0;

        if(strcmp(args[n2-1], "&") == 0) {
            free(args[n2]);
            args[n2-1] = NULL;
            n2--;

            flag_bg = 1;
        }

        for(int i=0; i<n2; i++) {
            if(args[i][0] == '~') {
                char* temp = (char*) calloc(const_size, sizeof(char));
                sprintf(temp, "%s/%s", root_dir, args[i] + 1);

                args[i] = temp;
            }
        }

// --------------------------------------------------------------------------------------------

        // Creating pipe

        if(pipe(fd) < 0) {
            perror("pipe: Creating pipe failed.");
            return;
        }

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

            // flushing input and output buffers
            fflush(stdin);
            fflush(stdout);

             if(prev_input != STDIN_FILENO) {
                dup2(prev_input, STDIN_FILENO);
                close(prev_input);
            }

            if(fd[1] != STDOUT_FILENO && i < n1 - 1) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }


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

//  --------------------------------------------------------------------------------------

            if(strcmp(args[0], "cd") == 0) {
                cd(n2, args);
            }
            else if(strcmp(args[0], "pwd") == 0) {
                pwd(n2, args);
            }
            else if(strcmp(args[0], "echo") == 0) {
                echo(n2, args);
            }
            else if(strcmp(args[0], "ls") == 0) {
                ls(n2, args);
            }
            else if(strcmp(args[0], "pinfo") == 0) {
                pinfo(n2, args);
            }
            else if(strcmp(args[0], "repeat") == 0) {
                repeat(n2, args, input_file, output_file, append);
            }
            else if(strcmp(args[0], "history") == 0) {
                history(n2, args);
            }
            else if(strcmp(args[0], "jobs") == 0) {
                jobs(n2, args);
            }
            else if(strcmp(args[0], "sig") == 0) {
                sig(n2, args);
            }
            else if(strcmp(args[0], "fg") == 0) {
                fg(n2, args);
            }
            else if(strcmp(args[0], "bg") == 0) {
                bg(n2, args);
            }
            else {
                int k = execvp(args[0], args); // Executing execvp, this will overwrite rest of the program if execution is successful.
                if(k < 0) {
                    perror("Command failed to execute.");
                    exit(1);
                }
                exit(0);
            }

            exit(0);
        }
        else {
            if(flag_bg) {
                printf("%d\n", pid);
                push_node(&head, pid, args[0]);
            }
            else {
                
                signal(SIGTTOU, SIG_IGN);
                signal(SIGTSTP, handler2);
                signal(SIGINT, handler3);
                

                // Assign foreground process group to the child process group
                // tcsetpgrp(0, pid);

                fg_pid = pid;
                strcpy(fg_name, args[0]);

                waitpid(pid, &status, WUNTRACED); // wait

                
                // tcsetpgrp(0, getpgid(0));

                signal(SIGINT, SIG_IGN);
                signal(SIGTSTP, SIG_IGN);
                signal(SIGTTOU, SIG_DFL);
            }

            close(fd[1]);
            prev_input = fd[0];
        }
    }
}