#include "headers.h"

void read_details(char* dir_name, int flag_a, int flag_l) {
    DIR* dir;
    struct dirent* file;
    struct stat st;

    struct passwd *pw;
    struct group *gr;

    dir = opendir(dir_name);

    if(dir == NULL) {
        fprintf(stderr, "ls: Directory not found.\n");
        closedir(dir);
        return;
    }

    stat(dir_name, &st);
    
    if(flag_l) {
        printf("total: %ld\n", (((st.st_blocks * st.st_blksize) + 512 - 1) / 512) * 2);
    }
    
    char* temp1 = (char*) calloc(const_size, sizeof(char));
    char* temp2 = (char*) calloc(const_size, sizeof(char));

    while((file = readdir(dir)) != NULL) {
        if(!flag_a && file->d_name[0] == '.') {
            continue;
        }

        sprintf(temp1, "%s/%s", dir_name, file->d_name);
        stat(temp1, &st);

        if(flag_l) {
            printf((S_ISDIR(st.st_mode)) ? "d" : "-");
            printf((st.st_mode & S_IRUSR) ? "r" : "-");
            printf((st.st_mode & S_IWUSR) ? "w" : "-");
            printf((st.st_mode & S_IXUSR) ? "x" : "-");
            printf((st.st_mode & S_IRGRP) ? "r" : "-");
            printf((st.st_mode & S_IWGRP) ? "w" : "-");
            printf((st.st_mode & S_IXGRP) ? "x" : "-");
            printf((st.st_mode & S_IROTH) ? "r" : "-");
            printf((st.st_mode & S_IWOTH) ? "w" : "-");
            printf((st.st_mode & S_IXOTH) ? "x" : "-");
            printf(" ");
            
            printf("%5lu ", st.st_nlink);

            pw = getpwuid(st.st_uid);
            printf("%s ", pw->pw_name);

            gr = getgrgid(st.st_gid);
            printf("%s ",gr->gr_name);

            printf("%10ld ", st.st_size);
            
            if (time(0) - st.st_mtime < 15780000) {
                strftime(temp2, const_size, "%b %e %H:%M", localtime(&st.st_mtime));
            }
            else {
                strftime(temp2, const_size, "%b %e %Y", localtime(&st.st_mtime));
            }

            printf("%s ", temp2);
        }

        printf(ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, file->d_name);
    }

    printf("\n");
    closedir(dir);
}