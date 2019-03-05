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

    while ((linelen = getline(&line, &linesize, stdin)) != -1) 
    {
        printf("line: %s", line);
        if (strncmp("exit", line, 4) == 0) 
        {
            exit(0); // user wants to exit
        }
        
        // fwrite(line, linelen, 1, stdout);
        // fork
        // if child, exec
        // if parent, wait
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

                char *myargs[command_size + 1];
                getArgList(myargs, line, command_size);

                execv(myargs[0], myargs);
            }

            // user input 'cd' command
            else if(strncmp("cd", line, 2) == 0)
            {
                char *myargs[2];
                myargs[0] = strdup("cd");
                myargs[1] = NULL;

                execv(myargs[0], myargs);
            }
            else
            {
                printf("%.*s is not a valid command.\n", (int)strlen(line)-1, line);
            }
        }
        else 
        {
            // parent goes down this path (original process)
            //printf("Hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
            wait(NULL);
            //printf("Parent finished.\n");
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