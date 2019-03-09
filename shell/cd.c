#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>

// Function to remove all spaces from a given string 
void removeSpaces(char *str) 
{ 
    // To keep track of non-space character count 
    int count = 0; 
  
    // Traverse the given string. If current character 
    // is not space, then place it at index 'count++' 
    for (int i = 0; str[i]; i++) 
        if (str[i] != ' ' && str[i] != '\n') 
            str[count++] = str[i]; // here count is 
                                   // incremented 
    str[count] = '\0'; 
} 

int pathExists(char *path)
{
    char *test = strtok(getenv("PATH"), ":");
    while(test != NULL)
    {
        printf("test: %s\n", test);
        printf("path: %s\n", path);
        printf("%d\n", !strcmp(test, path));
        if(!strcmp(test, path))
        {
            printf("SAME\n");
            break;
        }
        test = strtok(NULL, ":");
    }
}

int main(int argc, char *argv[])
{
    printf("cd: change directory\n");
    printf("arguments: %d\n", argc);
    for(int i = 0; i < argc; i++)
    {
        printf("argument #%d: %s\n", i, argv[i]);
    }

    char wd[100];

    removeSpaces(argv[1]);
    if(pathExists(argv[1]))
    {
        if(chdir(argv[1]) != 0)
        {
            perror("chdir() from user input failed");
        }
        printf("chdir success. path: %s", getcwd(wd, 100));
    }
}