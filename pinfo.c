#include "headers.h"

void pinfo(int argc, char **argv) {
    if(argc > 2) {
        fprintf(stderr, "pinfo: Command execution failed due to more than 1 argument.\n");
        return;
    }

    char* file_name = (char*) calloc(const_size, sizeof(char));
    if(argc == 1) {
        sprintf(file_name, "/proc/%d/stat", getpid());
    }
    else {
        sprintf(file_name, "/proc/%s/stat", argv[1]);
    }

    FILE* fp1 = fopen(file_name, "r");
    if(fp1 == NULL) {
        perror("pinfo: No file or directory referring to that process not found.\n");
        return;
    }

    char* temp = (char*) calloc(const_size, sizeof(char));

    int n1 = fread(temp, sizeof(char), const_size, fp1);
    if(ferror(fp1) != 0) {
        fprintf(stderr, "pinfo: Failed to read the process file.\n");
        return;
    }

    temp[n1+1] = '\0';
    fclose(fp1);

    char** values = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        values[i] = (char*) calloc(const_size, sizeof(char));
    }

    char delimit[] = " \t\r\n\v\f";
    int n2 = parse(temp, values, delimit);

    int flag = strcmp(values[5 - 1], values[8 - 1]); // process group id (pgrp) == foreground process group (tpid) == [foreground process]
    

    printf("PID -- %s\n", values[1 - 1]);

    printf("Process Status -- %s", values[3 - 1]);
    printf((flag == 0) ? "+\n" : "\n");

    printf("Virtual Memory Size (bytes) -- %s\n", values[23 - 1]); // Virtual memory size in bytes

    sprintf(file_name, "/proc/%s/exe", values[1 - 1]);
    readlink(file_name, temp, const_size);

    printf("Executable Path -- %s\n", temp);
}