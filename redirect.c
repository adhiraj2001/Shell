#include "headers.h"

int redirect(int* argc, char** argv, char* input_file, char* output_file, int* append) {

    int flag_error = 0;

    for(int i=0; i<*argc; i++) {
        if(strcmp(argv[i], "<") == 0) {
            if(i == *argc - 1) {
                flag_error = 1;

                argv[i] = NULL;
                *argc = *argc - 1;

                break;
            }

            strcpy(input_file, argv[i+1]);

            for(int j=0; j<2; j++) {
                for(int k=i; k<*argc; k++) {
                    argv[k] = argv[k+1];
                }

                argv[*argc-1] = NULL;
                *argc = *argc - 1;
            }

            i--;
        }
        else if(strcmp(argv[i],"<<") == 0) {
            flag_error = 1;
            break;
        }
        else if(strcmp(argv[i], ">") == 0) {
            if(i == *argc - 1) {
                flag_error = 1;

                argv[i] = NULL;
                *argc = *argc - 1;

                break;
            }

            printf("%s\n", argv[i+1]);

            strcpy(output_file, argv[i+1]);

            for(int j=0; j<2; j++) {
                for(int k=i; k<*argc; k++) {
                    argv[k] = argv[k+1];
                }

                argv[*argc-1] = NULL;
                *argc = *argc - 1;
            }

            i--;
        }
        else if(strcmp(argv[i], ">>") == 0) {
            if(i == *argc - 1) {
                flag_error = 1;

                argv[i] = NULL;
                *argc = *argc - 1;

                break;
            }

            *append = 1;

            strcpy(output_file, argv[i+1]);

            for(int j=0; j<2; j++) {
                for(int k=i; k<*argc; k++) {
                    argv[k] = argv[k+1];
                }

                argv[*argc-1] = NULL;
                *argc = *argc - 1;
            }

            i--;
        }
        else if(strlen(argv[i]) > 1) {
            if(argv[i][0] == '<') {
                if(argv[i][1] == '<') {
                    flag_error = 1;
                    break;
                }

                strcpy(input_file, argv[i] + 1);
            }
            else if(argv[i][0] == '>') {
                if(argv[i][1] == '>') {
                    *append = 1;
                }

                strcpy(output_file, argv[i] + 1 + *append);
            }
            else {
                continue;
            }

            for(int k=i; k<*argc; k++) {
                argv[k] = argv[k+1];
            }

            argv[*argc-1] = NULL;
            *argc = *argc - 1;
        }
    }

    return flag_error;
}