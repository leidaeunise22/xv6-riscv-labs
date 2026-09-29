#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int start, pid, elapsed;

  if(argc < 2){
    fprintf(2, "usage: time1 command [args ...]\n");
    exit(1);
  }

  start = uptime();
  pid = fork();
  if(pid < 0){
    fprintf(2, "time1: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // Skip time1 so the command receives its own name and arguments.
    exec(argv[1], &argv[1]);
    fprintf(2, "time1: exec %s failed\n", argv[1]);
    exit(1);
  }

  if(wait(0) < 0){
    fprintf(2, "time1: wait failed\n");
    exit(1);
  }
  elapsed = uptime() - start;
  printf("elapsed time: %d ticks\n", elapsed);
  exit(0);
}
