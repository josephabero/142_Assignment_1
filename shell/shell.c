#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int getCommandSize(char *line);
void getArgList(char* argList[], char *line, int command_size);

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t linesize = 0;
    ssize_t linelen;

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
    
    while ((linelen = getline(&line, &linesize, stdin)) != -1) 
    {
        // Handle "exit" command
        if (strncmp("exit", line, 4) == 0) 
        {
            exit(0);
        }
        
        // fwrite(line, linelen, 1, stdout);
        int rc = fork();
        if (rc < 0) 
        {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        else if (rc == 0) {
            // IT IS CHILD PROCESS

            // user input 'path' command
            // 1. parse number of command line arguments
            // 2. retrieve command list and store in array
            // 3. execv()
            //          - call 'path' command
            //          - pass argument list

            // EX)  path ls cd exit

            //      char *myargs[5];
            //      myargs[0] = "path"
            //      myargs[1] = "ls"     
            //      myargs[2] = "cd"
            //      myargs[3] = "exit"
            //      myargs[4] = NULL
            if(strncmp("path", line, 4) == 0)
            {
                printf("path line: %s", line);
                int command_size = getCommandSize(line);

                if(command_size < 1)
                {
                    printf("command size < 1\n");
                }
                else
                {
                    char *myargs[command_size + 1];
                    getArgList(myargs, line, command_size);

                    execv(myargs[0], myargs);
                }
            }

            // user input 'cd' command
            else if(strncmp("cd", line, 2) == 0)
            {
                printf("cd line: %s", line);
                int command_size = getCommandSize(line);

                if(command_size < 1 || command_size > 1)
                {
                    printf("command size < 1\n");
                }
                else
                {
                    char *myargs[3];
                    getArgList(myargs, line, command_size);

                    execv(myargs[0], myargs);
                }
            }
            else
            {
                printf("%.*s is not a valid command.\n", (int)strlen(line)-1, line);
            }
        }
        else 
        {
            //printf("Hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
            wait(NULL);
        }
    }

    free(line);
    if (ferror(stdin))
    {
        err(1, "getline");
    }
}

int getCommandSize(char *line)
{
    int command_size = 0;
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == ' ') command_size++;
    }
    printf("command_size: %d\n", command_size + 1);
    return command_size;
}
void getArgList(char *argList[], char *line, int command_size)
{
    char *temp;
    temp = strtok(line, " ");
    for(int j = 0; j <= command_size; j++)
    {
        printf("temp: %s\n", temp);
        argList[j] = temp;
        temp = strtok(NULL, " ");
    }
    argList[command_size + 1] = NULL;
}