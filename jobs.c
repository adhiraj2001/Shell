#include "headers.h"

void jobs(int argc, char** argv) {

    optind = 0; 
    optarg = NULL; 
    optopt = 0;

    int c;
    int flag_r = 0, flag_s = 0, flag_error = 0;
    while ((c = getopt(argc, argv, ":: r :: s ::")) != -1) {
        switch(c) {
            case 'r':
                flag_r++;
                break;
            case 's':
                flag_s++;
                break;
            case '?':
                flag_error++;
                fprintf(stderr, "jobs: Invalid option: '%c'\n", optopt);
                break;
            default:
                fprintf(stderr, "jobs: Usage: jobs [-flags]\n");
                return;
        }
    }

    if(flag_error) {
        fprintf(stderr, "jobs: Command execution failed due to incorrect flags. \n");
        return;
    }

    if(optind < argc) {
        fprintf(stderr, "jobs: Command execution failed because jobs command doesn't take arguments other than flags. \n");
        fprintf(stderr, "Usage: jobs [-flags]\n");
        return;
    }

// --------------------------------------------------------------------------------

    if(flag_r == 0 && flag_s == 0) {
        flag_r = 1; 
        flag_s = 1;
    }


    char** values = (char**) calloc(const_size, sizeof(char*));
    for(int i=0; i<const_size; i++) {
        values[i] = (char*) calloc(const_size, sizeof(char));
    }

    char* arr[const_size][2];
    
    int pid, status, pno = 0, sz = 0;
    struct Node* p = head;

    while(p != NULL) {

        pid = waitpid(p->pid, &status, WNOHANG);
        
        if(pid == 0) {

            pinfo2(p->pid, values);

            // if(strcmp(values[5 - 1], values[8 - 1]) == 0) { | No need
            //     p = p->next;
            //     continue;
            // } 

            pno++;
            
            char* process = (char*) calloc(const_size, sizeof(char));
            char* name = (char*) calloc(const_size, sizeof(char));

            if(flag_r && (strcmp(values[3 - 1], "R") == 0 || strcmp(values[3 - 1], "S") == 0)) {
                sprintf(process, "[%d] Running %s [%d]", pno, p->name, p->pid);
                sprintf(name, "%s%d", p->name, p->pid);

                arr[sz][0] = name;
                arr[sz++][1] = process;
            }
            else if(flag_s && strcmp(values[3 - 1], "T") == 0) {
                sprintf(process, "[%d] Stopped %s [%d]", pno, p->name, p->pid);
                sprintf(name, "%s%d", p->name, p->pid);

                arr[sz][0] = name;
                arr[sz++][1] = process;
            }
        }
        
        p = p->next;
    }

// ---------------------------------------------------------------------

    char* name = (char*) calloc(const_size, sizeof(char));
    char* process = (char*) calloc(const_size, sizeof(char));

    // sorting
    for(int i=0; i < sz; i++){
        for(int j=0; j < sz - 1 - i; j++){

            if(strcmp(arr[j][0], arr[j+1][0]) > 0){
                strcpy(name, arr[j][0]);
                strcpy(process, arr[j][1]);

                strcpy(arr[j][0], arr[j+1][0]);
                strcpy(arr[j][1], arr[j+1][1]);

                strcpy(arr[j+1][0], name);
                strcpy(arr[j+1][1], process);
            }
        }
    }

// --------------------------------------------------------------------

    for(int i=0; i<sz; i++) {
        printf("%s\n", arr[i][1]);
    }
}   
