#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  uint start, elapsed, percent;
  struct rusage usage;

  if(argc < 2){
    fprintf(2, "usage: time command [args ...]\n");
    exit(1);
  }

  start = uptime();
  pid = fork();
  if(pid < 0){
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    exec(argv[1], &argv[1]);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  if(wait2(0, &usage) < 0){
    fprintf(2, "time: wait2 failed\n");
    exit(1);
  }
  elapsed = (uint)uptime() - start;
  percent = elapsed ? (uint)((uint64)usage.cputime * 100 / elapsed) : 0;
  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
         elapsed, usage.cputime, percent);
  exit(0);
}
