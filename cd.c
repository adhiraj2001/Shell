#include "headers.h"

void cd(int argc, char** argv) {
    if(argc > 2) {
        fprintf(stderr, "cd: Takes only 1 argument.\n");
        return;
    }

    if(argc == 1 || strcmp(argv[1], "~") == 0) {
        int flag = chdir(root_dir);

        if(flag < 0) {
            fprintf(stderr, "cd: Incorrect number of arguments or incorrect path.\n");
            return;
        }

        strcpy(prev_dir, curr_dir);
        getcwd(curr_dir, const_size);
    }
    else if(strcmp(argv[1], "-") == 0) {
        int flag = chdir(prev_dir);

        if(flag < 0) {
            fprintf(stderr, "cd: Incorrect number of arguments or incorrect path.\n");
            return;
        }

        printf("%s\n", prev_dir); // bash also prints this

        strcpy(prev_dir, curr_dir);
        getcwd(curr_dir, const_size);
    }
    else if(argv[1][0] == '~') {
        char* temp = (char*) calloc(const_size, sizeof(char));
        sprintf(temp, "%s/%s", root_dir, argv[1] + 1);

        int flag = chdir(temp);

        if(flag < 0) {
            fprintf(stderr, "cd: Incorrect number of arguments or incorrect path.\n");
            return;
        }

        strcpy(prev_dir, curr_dir);
        getcwd(curr_dir, const_size);
    }
    else {
        int flag = chdir(argv[1]);

        if(flag < 0) {
            fprintf(stderr, "cd: Incorrect number of arguments or incorrect path.\n");
            return;
        }

        strcpy(prev_dir, curr_dir);
        getcwd(curr_dir, const_size);
    }
}