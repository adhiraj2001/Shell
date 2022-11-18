#include "headers.h"

void prompt() {
    struct utsname sys;
    uname(&sys);

    uid_t uid = geteuid(); // effective user id
    struct passwd *pw = getpwuid(uid); // password database linked to uid

    printf(ANSI_COLOR_CYAN "<%s@%s:" ANSI_COLOR_RESET, pw->pw_name, sys.sysname);

//  -----------------------------------------------------

    int n1 = strlen(root_dir);
    getcwd(curr_dir, const_size); // current working directory

    int n2 = strlen(curr_dir);
    
    if(strstr(curr_dir, root_dir) == curr_dir) {
        printf(ANSI_COLOR_YELLOW "~%s> " ANSI_COLOR_RESET, curr_dir + n1);
    }
    else {
        printf(ANSI_COLOR_YELLOW "%s> " ANSI_COLOR_RESET, curr_dir);
    }
}
