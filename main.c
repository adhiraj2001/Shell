#include "headers.h"

// Declating the extern variables

int const_size = 1000;

char* root_dir;
char* prev_dir;
char* curr_dir;

struct Node* head = NULL;

char* hist_v[20];
int hist_c;

int standard_input;
int standard_output;

int fg_pid;
char* fg_name;

// -------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {

    // Initializing the extern variables.

    root_dir = (char*) calloc(const_size, sizeof(char));
    getcwd(root_dir, const_size);

    prev_dir = (char*) calloc(const_size, sizeof(char));
    strcpy(prev_dir, root_dir);

    curr_dir = (char*) calloc(const_size, sizeof(char));
    strcpy(curr_dir, root_dir);

    char* command_line = (char*) calloc(const_size, sizeof(char));
    size_t command_size = const_size;
    size_t input_chars = 0;

    char** commands = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        commands[i] = (char*) calloc(const_size, sizeof(char));
    }

    char** args = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        args[i] = (char*) calloc(const_size, sizeof(char));
    }

    hist_c = 0;
    for(int i=0; i<20; i++) {
        hist_v[i] = (char*) calloc(const_size, sizeof(char));
    }

    // making a copy file descriptor for stdin and stdout
    standard_input = dup(STDIN_FILENO);
    standard_output = dup(STDOUT_FILENO);


    // storing running fg processes
    fg_pid = 0;
    fg_name = (char*) calloc(const_size, sizeof(char));

// -------------------------------------------------------------------------------------

    // prevents our shell to be interrupted by ctrl + z
    signal(SIGTSTP, SIG_IGN);

    // prevents our shell to be killed by ctrl + c
    signal(SIGINT, SIG_IGN);

    // handling if any of background child process has been exited
    signal(SIGCHLD, handler1);

// ------------------------------------------------------------------------------------

    while(1) {

        prompt(root_dir);

        input_chars = getline(&command_line, &command_size, stdin); // can also use getline()
        command_line[strlen(command_line)-1] = '\0';

        if(input_chars == -1) {
            perror(ANSI_COLOR_GREEN "Shell Exit" ANSI_COLOR_RESET);
            exit(0);
        }

// -------------------------------------------------------------------------------------

        // inserting history
        if(hist_c >= 20) {
            for(int i=0; i<19; i++) {
                strcpy(hist_v[i], hist_v[i+1]);
            }

            strcpy(hist_v[19], command_line);
        }
        else {
            strcpy(hist_v[hist_c], command_line);
            hist_c++;
        }

// --------------------------------------------------------------------------------------

        // parsing command line arguments by ';'
        int n1 = parse(command_line, commands, ";");

        for(int i=0; i<n1; i++) {

            int flag_piped = 0;
            for(int j=0; j<strlen(commands[i]); j++) {
                if(commands[i][j] == '|') {
                    flag_piped = 1;
                    break;
                }
            }

            if(flag_piped) {
                piped_command(commands[i]);
                continue;
            }

// ---------------------------------------------------------------------------------------

            // parsing each command and removing the whitespaces.
            char delimit[] = " \t\r\n\v\f";
            int n2 = parse(commands[i], args, delimit);

            if(n2 == 0) {
                continue;
            }

            if(strcmp(args[0], "exit") == 0) {
                exit(0);
            }

            int append = 0;
            char* input_file = (char*) calloc(const_size, sizeof(char));
            char* output_file = (char*) calloc(const_size, sizeof(char));

            int flag_error = redirect(&n2, args, input_file, output_file, &append);

            if(flag_error) {
                fprintf(stderr, "><: Incorrect syntax for input - output redirection.\n");
                continue;
            }

            if(n2 == 0) {
                fprintf(stderr, "Syntax error: No command found.\n");
                continue;
            }

// -------------------------------------------------------------------------------------

            // flushing input and output buffers
            fflush(stdin);
            fflush(stdout);

            // input-output redirection

            if(strlen(input_file) > 0) {
                int fd = open(input_file, O_RDONLY, 0644);
                if(fd < 0) {
                    perror("input_file: Failed to open.");
                    continue;
                }

                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if(strlen(output_file) > 0) {

                int fd = -1;
                
                if(append) {
                    fd = open(output_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
                }
                else {
                    fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                }

                if(fd < 0) {
                    perror("output_file: Failed to open.");
                    continue;
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

//  ----------------------------------------------------------------------------------------

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
                execute(n2, args, input_file, output_file, append);
            }

            dup2(standard_input, STDIN_FILENO);
            dup2(standard_output, STDOUT_FILENO);  
        }
    }

    close(standard_input);
    close(standard_output);

    return 0;
}
