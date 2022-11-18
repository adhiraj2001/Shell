#include "headers.h"

void repeat(int argc, char** argv, char* input_file, char* output_file, int append) {
    char *str, *endptr;
    long val;

    if (argc < 2) {
        fprintf(stderr, "repeat: No arguments found.\n");
        return;
    }

    str = argv[1];

    errno = 0;
    val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0)) {
        perror("repeat: Invalid arguments. Either not integer or overflow.");
        return;
    }

    if (endptr == str || *endptr != '\0') {
        fprintf(stderr, "repeat: Characters other than numbers found.\n");
        return;
    }

    argc -= 2;

    for(int i=0; i<val; i++) {
        if(strcmp(argv[2], "cd") == 0) {
            cd(argc, argv + 2);
        }
        else if(strcmp(argv[2], "pwd") == 0) {
            pwd(argc, argv + 2);
        }
        else if(strcmp(argv[2], "echo") == 0) {
            echo(argc, argv + 2);
        }
        else if(strcmp(argv[2], "ls") == 0) {
            ls(argc, argv + 2);
        }
        else if(strcmp(argv[2], "pinfo") == 0) {
            pinfo(argc, argv + 2);
        }
        else if(strcmp(argv[2], "repeat") == 0) {
            repeat(argc, argv + 2, input_file, output_file, append);
        }
        else if(strcmp(argv[2], "history") == 0) {
            history(argc, argv + 2);
        }
        else if(strcmp(argv[2], "jobs") == 0) {
            jobs(argc, argv + 2);
        }
        else if(strcmp(argv[2], "sig") == 0) {
            sig(argc, argv + 2);
        }
        else if(strcmp(argv[2], "fg") == 0) {
            fg(argc, argv + 2);
        }
        else if(strcmp(argv[2], "bg") == 0) {
            bg(argc, argv + 2);
        }
        else {
            execute(argc, argv + 2, input_file, output_file, append);
        }
    }
}