#include "headers.h"

void pwd(int argc, char** argv) {
    if(argc > 1) {
        fprintf(stderr, "pwd: Takes only no arguments.\n");
        return;
    }

    printf("%s\n", curr_dir);
}