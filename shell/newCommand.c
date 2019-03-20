#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> 

int getNumberOfPaths(char *line);
void getEachPath(char* argList[], char *line, int command_size);

int main(int argc, char *argv[])
{
    // global path is stored in the last element of argv array
    char *global_path = argv[argc - 1];


    printf("path: provides path to commands.\n");
    // printf("arguments: %d\n", argc);
    // printf("global_path: %s\n", global_path);

    // for(int i = 0; i < argc; i++)
    // {
    //     printf("argument #%d: %s\n", i, argv[i]);
    // }

    int num_of_paths = getNumberOfPaths(global_path);
    char *path_list[num_of_paths + 1];

    // Stores each path in 'path_list'
    getEachPath(path_list, global_path, num_of_paths);

    // create argument array with number of paths to iterate through
    char *myargs[num_of_paths];

    // path pointer with memory allocated based on the length of the string
    char *pathToCheck = (char *)malloc((sizeof(path_list)/sizeof(path_list[0])) + strlen(argv[0]) + 2);

    // local variables to be used in process
    char working_directory[1000];
    getcwd(working_directory, sizeof(working_directory));
    bool valid_command = false;


    for(int i=0; i <= num_of_paths; i++)
    {
        printf("path to check: %s/%s\n", path_list[i], argv[0]);
        strcpy(pathToCheck, path_list[i]);
        printf("pathToCheckFirst: %s\n", pathToCheck);
        strcat(pathToCheck, "/");
        strcat(pathToCheck, argv[0]);
        printf("pathToCheck: %s\n", pathToCheck);
        myargs[0] = working_directory;

        if(access(pathToCheck, X_OK) == 0)
        {
            printf("Valid Address %s\n", pathToCheck);
            execvp(pathToCheck, myargs);
            valid_command = true;
            break;
        }
        strcpy(pathToCheck, "");
    }

    if(!valid_command)
    {
        printf("NOT A VALID COMMAND\n");
    }

    return 1;
}



int getNumberOfPaths(char *line)
{
    int command_size = 0;
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == ' ') command_size++;
    }
    //printf("command_size: %d\n", command_size + 1);
    return command_size;
}
void getEachPath(char *argList[], char *line, int command_size)
{
    char *temp;
    temp = strtok(line, " ");
    for(int j = 0; j <= command_size; j++)
    {
        //printf("temp: %s\n", temp);
        argList[j] = temp;
        temp = strtok(NULL, " ");
    }
    argList[command_size + 1] = NULL;
}