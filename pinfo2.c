#include "headers.h"

int pinfo2(int pid, char** values) {

    char* file_name = (char*) calloc(const_size, sizeof(char));

    sprintf(file_name, "/proc/%d/stat", pid);

    FILE* fp1 = fopen(file_name, "r");
    if(fp1 == NULL) {
        perror("pinfo2: No file or directory referring to that process not found.\n");
        return 0;
    }

    char* temp = (char*) calloc(const_size, sizeof(char));

    int n1 = fread(temp, sizeof(char), const_size, fp1);
    if(ferror(fp1) != 0) {
        fprintf(stderr, "pinfo2: Failed to read the process file.\n");
        return 0;
    }

    temp[n1+1] = '\0';
    fclose(fp1);

    char delimit[] = " \t\r\n\v\f";
    int n2 = parse(temp, values, delimit);

    return n2;
}