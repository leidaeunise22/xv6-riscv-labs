#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
#define MAXARGS 16

int 
main(int argc, char *argv[])
{
    int i, rc;
    char *newargv[MAXARGS];
    int priority;

    if(argc < 3 || argc - 2 >= MAXARGS){
        printf("Usage: pexec <priority> <command> [args...]\n");
        exit(-1);
    }

    priority = atoi(argv[1]);
    if(setpriority(priority) < 0){
        fprintf(2, "pexec: priority must be 0 through 49\n");
        exit(1);
    }
    for(i=2; i<argc; i++)
    newargv[i-2] = argv[i];
    newargv[argc-2] = 0;

    rc = fork();
    if(rc < 0){
        fprintf(2, "pexec: fork failed\n");
        exit(1);
    }
    if(rc == 0){
        exec(newargv[0], newargv);
        fprintf(2, "pexec: exec failed\n");
        exit(1);
    }
    int status;
    wait(&status);
    exit(status);
}
