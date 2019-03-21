#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

int getCommandSize(char *line);
void getArgList(char* argList[], char *line, int command_size);

int getNumberOfPaths(char *line);
void getEachPath(char* argList[], char *line, int command_size);

int newCommand(char *paths[]);

char globalPath[1000];

int main(int argc, char **argv)
{
    strcpy(globalPath, "/bin");
    char *line = NULL;
    size_t linesize = 0;
    ssize_t linelen;


    bool exit_cond = false;

    /* Develop built in commands:
        1. exit
        2. path
        3. cd
    
        exit:
            When the user types 

                                    exit

            your shell should simply call the exit system call with 0 
            as a parameter. It is an error to pass any arguments to exit.

        path:
            The path command takes 0 or more arguments, 
            with each argument separated by whitespace from the others. 
            A typical usage would be like this: 

                            wish> path /bin /usr/bin

            this would add '/bin' and '/usr/bin' to the search path of the shell.

            If the user sets path to be empty, then the shell should not be able 
            to run any programs (except built-in commands). 
            The path command always overwrites the old path with the newly specified path.

        cd:
            cd always take one argument (0 or >1 args should be signaled as an error). 
            To change directories, use the chdir() system call with the argument supplied 
            by the user; 
            if chdir fails, that is also an error.


        Process Steps: 
            1. Fork
            2. Child or Parent?
                A: Child 
                    Determine 'path' or 'cd' command
                    Save user arguments
                    Exec()
                B: Parent
                    End process
    */
    printf("CShell> ");
    while ((linelen = getline(&line, &linesize, stdin)) != -1) 
    {
        // Handle "exit" command
        if (strncmp("exit", line, 4) == 0) 
        {
            exit_cond = true;
            exit(0);
            break;
        }
        
        // fwrite(line, linelen, 1, stdout);
        int rc = fork();
        if (rc < 0) 
        {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(0);
        }
        else if (rc == 0) 
        {
            line = strtok(line, "\n");
            // IT IS CHILD PROCESS

            // user input 'path' command
            // 1. parse number of command line arguments
            // 2. retrieve command list and store in array
            // 3. execv()
            //          - call 'path' command
            //          - pass argument list

            // EX)  path /bin /bin/usr /bin/temp

            //      char *myargs[5];
            //      myargs[0] = "path"
            //      myargs[1] = "/bin"     
            //      myargs[2] = "/bin/usr"
            //      myargs[3] = "/bin/temp"
            //      myargs[4] = NULL
            if(strncmp("path", line, 4) == 0)
            {
                char *temp = line;
                int i = 0;
                temp += 5;
                strcpy(globalPath, temp);
                printf("New Global Path: %s\n", globalPath);
            }

            // user input 'cd' command
            else if(strncmp("cd", line, 2) == 0)
            {
                char *temp;
                temp = line + 3;              // removes 'cd ' from temp
                if(access(temp, F_OK) == 0)
                {
                    chdir(temp);
                }
                else
                {
                    printf("Path DOES NOT exist. %s\n", temp);
                }
                char cwd[1000];
                printf("Current Directory: %s\n", getcwd(cwd, sizeof(cwd)));
            }
            else
            {
                int command_size = getCommandSize(line);

                char *myargs[command_size + 2];
                getArgList(myargs, line, command_size);

                if(newCommand(myargs))
                {
                    printf("INVALID COMMAND\n");
                }
            }
        }
        else 
        {
            //printf("Hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
            wait(NULL);
            // printf("parent\n");
            // char working_directory[1000];
            // getcwd(working_directory, sizeof(working_directory));
            // printf("wd: %s\n", working_directory);
        }
        printf("CShell> ");
    }

    free(line);
    if (ferror(stdin))
    {
        err(1, "getline");
    }
    exit(1);
}

int getCommandSize(char *line)
{
    int command_size = 0;
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == ' ') command_size++;
    }
    return command_size;
}
void getArgList(char *argList[], char *line, int command_size)
{
    char *temp;
    temp = strtok(line, " ");
    for(int j = 0; j <= command_size; j++)
    {
        argList[j] = temp;
        temp = strtok(NULL, " ");
    }
    argList[command_size + 1] = NULL;
}

int getNumberOfPaths(char *line)
{
    int command_size = 0;
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == ' ') command_size++;
    }
    return command_size;
}

void getEachPath(char *argList[], char *line, int command_size)
{
    char *temp;
    temp = strtok(line, " ");
    for(int j = 0; j <= command_size; j++)
    {
        argList[j] = temp;
        temp = strtok(NULL, " ");
    }
    argList[command_size + 1] = NULL;
}

int newCommand(char *paths[])
{
    int invalid_command = 1;
    // Gets numbers of paths
    // Ex) globalPath = '/bin /usr /temp'
    //     num_of_paths = 2     (includes 0)
    int num_of_paths = getNumberOfPaths(globalPath);

    // Creates char* array holding each path
    // Ex) 
    //      path_list[0] = '/bin'
    //      path_list[1] = '/usr'
    //      path_list[2] = '/temp'
    char *path_list[num_of_paths + 1];
    getEachPath(path_list, globalPath, num_of_paths);

    // path pointer with memory allocated based on the length of the string
    char *pathToCheck = (char *)malloc((sizeof(path_list)/sizeof(path_list[0])) + strlen(globalPath) + 2);

    // iterates through each path and checks to see if it's a valid path.
    // if path exists, then it exec() to execute command
    // else, it will iterate through the next path

    // if all paths are invalid, error message will be displayed.

    // Example
    // -------
    // CShell> path /bin /usr/ temp
    // CShell> ls

    // pathToCheck = '/bin'
    // pathToCheck = '/bin/'
    // pathToCheck = '/bin/ls'
    // Does '/bin/ls' exist?
    // Yes, execute command
    for(int i=0; i <= num_of_paths; i++)
    {
        strcpy(pathToCheck, path_list[i]);
        strcat(pathToCheck, "/");
        strcat(pathToCheck, paths[0]);
        // paths[0] = working_directory;

        if(access(pathToCheck, X_OK) == 0)
        {
            execv(pathToCheck, paths); // creates new process to execute command program. passes in arguments
            invalid_command = 0;
            break;
        }
    }
}