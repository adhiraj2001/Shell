# OS_Assignment-3

## Running the file
* Go to the Assignment folder.
* Execute ***make*** on terminal. This complies all the .c files in 'shell' object file.
* Run the program by executing '**./shell**' in the terminal.

## Assignment tasks
* This part of the assignment focused on implementing input-output redirection and piping of processes.
* Also implemented signal handling for different types of signals and handling their effects on processes.

## Files and their functions:

[//]: # (Assignment - 2 added files)
> Assignment-2 added files but some are modified in Assignment-3
### headers.h:
* Contains all headers and header files and extern variable declaration.

### main.c:
* This file initiates our shell program.
* This file is also used to declare and initialize our *extern* variables.
* We also keep our *SIGNCHLD* signal handling in our main function.
* This has the shell loop to continously take user inputs from the terminal.
* Also redirects standard input and output according to input.

### prompt.c:
* This file is used to print the username and system name at the start of every command line.
* This also prints our current working directory in every command line.
* This assumes that execution directory of file is root directory.
* Also colors the termial output using ANSI color codes.

### parse.c:
* It parses the given argument with given delimiter.

### cd.c:
* Implements cd function
* Makes the use of curr_dir, prev_dir and root_dir extern variables to implement previous '**-**' directory cd command.
* This assumes that execution directory of file is root directory.

### pwd.c:
* prints current working directory.

### echo.c:
* prints everything written after echo after removing extra white spaces.
* Uses parse.c to remove extra whitespaces.

### ls.c:
* Lists the contents and details of the given directory according to flags *-a*, *-l*.
* Makes use of ***getopt()*** function to parse the input to flags and arguments.
* Uses read_details.c file to print all the details of the contents of the given directory/file according to flags.

### read_details.c:
* Uses ***struct dirent***, ***struct stat*** to print the details of the contents accoring to the flags.
* Replicates the content shown by bash terminal.

### pinfo.c:
* Prints details of the given process id or shell using ***/proc/[pid]/stat*** and ***/proc/[pid]/exe*** file details.
* Uses fopen in read to access these files.

### repeat.c:
* Executes the argument command, about given argument number of times.

### history.c:
* Gives the history of the recent executed commands up untill last 20 commands.
* Assumes history reset after each execution of shell.

### execute.c:
* Execute System commands using fork and execvp.
* Also handles background processes.
* Also outputs exit status of background process using linked list.
* Also handles signals and input-output redirection for child process.

### list.c:
* Implements linked list to store pids and names of running background processes.

### handler.c:
* Handler function for ***SIGCHLD** signal in main function, prints the exit status of our background child process.
* Uses linked list to iterate through process pids and checks exited pids.

---

[//]: # (Assignment - 3 added files)
> Assignment - 3 added files:

### redirect.c:
* Parses the input command to extract input and output files according to symbols '**<**', '**>**', '**>>**'.

### piped_command.c
* Handles input commands with piping.
* First parses the command input according to '**|**' pipe symbol, then uses ***pipe()*** and ***dup2*** command to redirect standard input and output for every command execution which is used as input by the next command.

### pinfo2.c
* Extract values from ***/proc/[pid]/stat*** according to pid.

### jobs.c
* Prints all the active background processes with their status and pid.
* Iterates through the list of bg pids and uses ***pinfo2()*** to extract status information.

### sig.c
* Uses ***kill()*** command to send signal according to pid.

### fg.c
* Uses ***kill()*** command to send **SIGCONT** to change bg process state from *stopped* to *running* and then bring the process to foreground and then wait for it's execution.

### bg.c
* Uses ***kill()*** command to send **SIGCONT** to change bg process state from *stopped* to *running* but this time keep the process in background.

### handler2.c
* Handles **SIGTSTP** signal to send foreground process to background and change their state to *stopped* but keeping our shell unaffected.

### handler3.c
* Handles **SIGINT* signal to kill foreground process but keeping out shell execution unaffected.

### replay.c
* This file implements the bonus task which executes a command in intervals over a certain period.
* This was implemented by using forloop for a certain period of input time, and then using sleep function between every iteration we can replicate the periods.