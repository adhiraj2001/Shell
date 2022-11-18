#include "headers.h"

int parse(char* command_line, char** args, char delimit[]) {
    int pos = 0;
    if(command_line != NULL) {
        args[pos] = command_line;

        char* temp = strtok(command_line, delimit);
        while(temp != NULL) {
            args[pos++] = temp;
            temp = strtok(NULL, delimit);
        }
    }

    args[pos] = NULL;
    
    return pos;
}