#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
    char *line = NULL;
    size_t linesize = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linesize, stdin)) != -1) 
    {
        if (strncmp("exit", line, 4) == 0) 
        {
            exit(0); // user wants to exit
        }
        
        // fwrite(line, linelen, 1, stdout);
        // fork
        // if child, exec
        // if parent, wait
        int rc = fork();
        if (rc < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            // child (new process)
            printf("hello, I am child (pid:%d)\n", (int) getpid());
            printf("I want to exec\n");
            sleep(1);
        } else {
            // parent goes down this path (original process)
            // int wc = wait(NULL);
            printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
            wait(NULL);
            printf("parent finished.\n");
        }
    }

    free(line);
    if (ferror(stdin))
    {
        err(1, "getline");
    }
}
