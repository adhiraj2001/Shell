#include "headers.h"

void echo(int argc, char** argv) {
    if(argc == 1) {
        printf("\n");
        return;
    }

    for(int i=1; i<argc; i++) {
        if(i > 1) {
            printf(" ");
        }

        printf("%s", argv[i]);
    }

    printf("\n");
}