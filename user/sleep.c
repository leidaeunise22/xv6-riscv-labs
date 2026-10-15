#include "kernel/types.h"
#include "user/user.h"

// Used to pause between commands when checking runnable ages with ps.
int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "usage: sleep ticks\n");
    exit(1);
  }
  for(int i = 0; argv[1][i]; i++){
    if(argv[1][i] < '0' || argv[1][i] > '9')
      exit(1);
  }
  if(sleep(atoi(argv[1])) < 0)
    exit(1);
  exit(0);
}
