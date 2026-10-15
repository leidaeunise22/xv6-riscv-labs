#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(void)
{
  static struct pstat procs[NPROC];
  int status;
  if(getpriority() != 0 || setpriority(17) < 0 || getpriority() != 17 ||
     setpriority(-1) != -1 || setpriority(50) != -1 || getpriority() != 17){
    printf("prioritytest: FAIL basic calls\n");
    exit(1);
  }
  int pid = fork();
  if(pid < 0)
    exit(1);
  if(pid == 0){
    if(getpriority() != 17 || setpriority(49) < 0 || getpriority() != 49)
      exit(1);
    exec("ps", (char *[]){"ps", 0});
    exit(1);
  }
  wait(&status);
  int n = getprocs(procs);
  int found = 0;
  for(int i = 0; i < n; i++)
    if(procs[i].pid == getpid() && procs[i].priority == 17)
      found = 1;
  if(status != 0 || !found || getpriority() != 17 ||
     getprocs((struct pstat *)0xffffffffffffffffL) != -1){
    printf("prioritytest: FAIL inheritance or process listing\n");
    exit(1);
  }
  setpriority(0);
  printf("prioritytest: PASS range, inheritance, isolation, ps, bad pointer\n");
  exit(0);
}
