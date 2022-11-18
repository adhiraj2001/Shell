#include <stdio.h> // fflush(), perror(), sprintf()
#include <stdlib.h> // calloc(), malloc()
#include <string.h> //  strtok(), strstr(), strlen()
#include <time.h>

#include <unistd.h> // system calls: open(), read(), write(), lseek(), gethostname()
#include <fcntl.h> // use of flags O_CREAT, SEEK_SET, etc.

#include <sys/stat.h> // mkdir(), stat(), _exit()
#include <sys/types.h>
#include <sys/wait.h> // wait()
#include <sys/utsname.h> // uname()

#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>

// Function Headfiles

#include "prompt.h"
#include "execute.h"
#include "parse.h"
#include "cd.h"
#include "pwd.h"
#include "echo.h"
#include "ls.h"
#include "read_details.h"

#include "pinfo.h"
#include "pinfo2.h"

#include "repeat.h"
#include "list.h"

#include "handler1.h"
#include "handler2.h"
#include "handler3.h"

#include "history.h"

#include "redirect.h"
#include "piped_command.h"
#include "jobs.h"
#include "sig.h"
#include "fg.h"
#include "bg.h"

// colors

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// extern variables

extern int errno;
extern int const_size;

extern char* root_dir;
extern char* prev_dir;
extern char* curr_dir;

extern char* RED;
extern char* NC;

extern struct Node* head;

extern char* hist_v[20];
extern int hist_c;

extern int standard_input;
extern int stadard_output;

extern int fg_pid;
extern char* fg_name;