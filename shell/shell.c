#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *globalPath;

int main(int argc, char **argv)
{
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

    /* variables used to store user_input from shell

        EX) CShell> path /bin /bin/usr

        line = "path /bin /bin/usr"
    */
    char *line = NULL;
    size_t linesize = 0;
    ssize_t linelen;

    printf("CShell> ");
    while ((linelen = getline(&line, &linesize, stdin)) != -1) 
    {
       line = strtok(line,"\n");
        // Handles "exit" command

        if (strncmp("exit", line, 4) == 0) 
        {
            exit(0);
        }
        
        
        int rc = fork();

        // FORK failed, EXIT with error
        if (rc < 0) 
        {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        // This is a CHILD PROCESS
        else if (rc == 0) 
        {
            // User input 'path command'
            if(strncmp("path", line, 4) == 0)
            {
                /*
                    'path' function
                    ---------------
                    Description:
                        globalPath variable is STORED from user arguments.
                        globalPath is used by other commands to check if command definition exists.

                    1) CShell> path /bin /bin/usr

                    GOAL:

                        globalPath = "/bin /bin/usr"
                
                    2) CShell> path

                    GOAL:

                        globalPath = ""

                        NOTE: non-user defined commands should NOT work. 
                              (only 'path', 'cd', and 'exit' commands should work with EMPTY path)
                */
                printf("'path' command! LINE: %s", line);
         		globalPath = line + 5;
         		printf("globalPath: %s", globalPath);
            }

            // user input 'cd' command
            else if(strncmp("cd", line, 2) == 0)
            {
                /*
                    'cd' function
                    ---------------
                    Description:
                        cd sets working directory of shell.
                        working directory is the CURRENT folder you are in while using the shell

                        NOTE: working directory is DIFFERENT from globalPath variable!
                            working directory is what commands like 'ls' will work in reference to. 
                            globalPath will hold all usable paths and is used to check if commands exist. 

                        'cd' command MUST only use 1 argument!
                        

                    Functionality:
                        EX) /mnt/c/142_Assignment_1/shell

                        1) CShell> cd /bin

                        GOAL:

                            IF /bin exists, set working directory to /bin

                        2) CShell> cd /path/that/does/not/exist

                        GOAL:

                            path is not real, do NOT change working directory

                        


                        HINT STEPS:
                            1. When processing command, remove 'cd ' from line input variable
                                EX) cd /bin

                                    store '/bin' in temp variable

                            2. Use access() command to check if variable exists

                                EX) access(//..temp variable with path..//, X_OK)

                                manual page is linked here: https://linux.die.net/man/2/access

                                if access(...) == 0, path exists
                                else, path does not exist

                            3. Use chdir() command to change working directory to valid path

                                EX) chdir(//..temp variable with path..//)

                                manual page linked here: http://man7.org/linux/man-pages/man2/chdir.2.html

                                if chdir(...) == 0, working directory was changed
                                else, changing directory failed

                            4. Print current working directory

                                Explanation: https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
                */
                printf("'cd' command! LINE: %s", line);
               // char *temp;

               // temp = line +3;
               // char s[100];

               // if(access(temp,F_OK)==0){
               // 	printf("Path exists");
               // 	chdir(temp);
               // 	if(chdir(temp)==0){
               // 		printf("Current Directory: %s\n",getcwd(s,sizeof(s)));
               // 	}
               // 	else{
               // 		printf("Changing directory failed.");
               // 	}
               // }
               // else{
               // 	printf("Path does not exist.");
               // }
                               char *temp;
                temp = line + 3;              // removes 'cd ' from temp
                //printf("cd line: %s, globalPath: %s\n", temp, globalPath);
                if(access(temp, F_OK) == 0)
                {
                    //printf("Path exists. cd to %s\n", temp);
                    chdir(temp);
                }
                else
                {
                    printf("Path DOES NOT exist. %s\n", temp);
                }
                char cwd[1000];
                printf("Current Directory: %s\n", getcwd(cwd, sizeof(cwd)));

            }
            /*
                Non-Built-In Command was input

                EX) CShell> ls

                EX) CShell> mkdir newFolder/

                    NOTE: expected to perform function of valid commands

                EX) CShell> command_that_doesnt_exist

                    NOTE: expected to print ERROR message that command doesn't exist
            */
            else
            {
                printf("NOT a built in command! LINE: %s", line);
                /*
                    Almost any command will use this
                    ---------------------------------
                    
                    globalPath will be used to check if command exists in a directory.
                    
                    GOAL:
                    
                        if command exists, exec() and execute the command with its arguments
                        else, print ERROR message

                    HINT steps for code within this ELSE statement:
                        1. Use execvp()

                            execvp("newCommand", line);

                            WHAT THIS DOES:
                                Creates new process executing 'newCommand' and passes in line (the user input) as an argument
                                (This assumes you built 'newCommand' with gcc)
                */
                char *myargs[1];
                myargs[0] = line;
                execv("newCommand", myargs);
            }
        }
        else 
        {
            wait(NULL);
            char wd[100];
            printf("PARENT. path: %s\n", getcwd(wd, 100));
        }
        printf("CShell> ");
    }

    free(line);
    if (ferror(stdin))
    {
        err(1, "getline");
    }
}