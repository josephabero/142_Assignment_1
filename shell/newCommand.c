#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("A NON-BUILT IN COMMAND WAS ENTERED\n");
    printf("User input line: %s\n", argv[0]);
    
    /*
    	HINT STEPS for newCommand.c:
                        1. Store command name from argv[0] variable (the line variable that was passed in)

                            EX) CShell> ls

                                store 'ls' in temp variable

                            EX) CShell> mkdir newFolder/

                                store 'mkdir' in temp variable

                        2. Break globalPath into individual paths

                            EX) CShell> globalPath = /bin /bin/usr/ /some/other/path

                                /bin
                                /bin/usr/
                                /some/other/path

                        3. Add '/<command name>' to path name

                            EX) CShell> ls

                                store '/bin/ls' in temp variable

                        4. Check if path exists

                            ** see step 2 of 'cd' command **

                        5. Conclude:

                            If path exists, exec()          //this executes the program to run the command


                            If path does NOT exist, REPEAT steps 3-5 for remaining globalPath paths

                        6. Print ERROR message that command is INVALID

                            ** only occurs IF EXEC() doesn't happen **
    */

    /*
		YOUR CODE GOES HERE
    */
}