#include "headers.h"

void ls(int argc, char** argv) {

    optind = 0; 
    optarg = NULL; 
    optopt = 0;

    int c;
    int flag_a = 0, flag_l = 0, flag_error = 0;
    while ((c = getopt(argc, argv, ":: a :: l ::")) != -1) {
        switch(c) {
            case 'a':
                flag_a++;
                break;
            case 'l':
                flag_l++;
                break;
            case '?':
                flag_error++;
                fprintf(stderr, "ls: Invalid option: '%c'\n", optopt);
                break;
            default:
                fprintf(stderr, "ls: Usage: ls [-flags] [optional arguments]\n");
                return;
        }
    }

    if(flag_error) {
        fprintf(stderr, "ls: Command execution failed due to incorrect flags. \n");
        return;
    }

// --------------------------------------------------------------------------------------------------------------

    if(optind >= argc) {
        read_details(curr_dir, flag_a, flag_l);
        return;
    }

    char* temp = (char*) calloc(const_size, sizeof(char));

    for(int i=optind; i<argc; i++) {
        if(strcmp(argv[i], ".") == 0) {
            printf(".:\n");
            read_details(curr_dir, flag_a, flag_l);
        }
        else if(strcmp(argv[i], "..") == 0) {
            sprintf(temp, "%s/%s", curr_dir, argv[i]);

            printf("..:\n");
            read_details(temp, flag_a, flag_l);
        }
        else if(argv[i][0] == '~') {
            sprintf(temp, "%s/%s", root_dir, argv[i] + 1);

            printf("%s:\n", argv[i]);
            read_details(temp, flag_a, flag_l);
        }
        else {
            printf("%s:\n", argv[i]);
            read_details(argv[i], flag_a, flag_l);
        }
    }
}