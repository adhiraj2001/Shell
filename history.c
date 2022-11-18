#include "headers.h"

void history(int argc, char** argv) {
    char *str, *endptr;
    long val;

    if (argc > 2) {
        fprintf(stderr, "history: Inavlid number of arguments found.\n");
        return;
    }

    if(argc == 1) {
        int k = (hist_c < 10) ? hist_c : 10;

        for(int i=hist_c-k; i<hist_c; i++) {
            printf("%s\n", hist_v[i]);
        }

        return;
    }

    str = argv[1];

    errno = 0;
    val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0)) {
        perror("history: Invalid arguments. Either not integer or overflow.");
        return;
    }

    if (endptr == str || *endptr != '\0') {
        fprintf(stderr, "history: Characters other than numbers found.\n");
        return;
    }

    if (val > 20) {
        fprintf(stderr, "history: Argument >20 invalid.\n");
        return;
    }

    int k = (hist_c < val) ? hist_c : val;
    
    for(int i=hist_c - k; i<hist_c; i++) {
        printf("%s\n", hist_v[i]);
    }
}