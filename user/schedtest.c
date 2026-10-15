#include "kernel/types.h"
#include "user/user.h"

struct result {
  int priority;
  int response;
  int turnaround;
};

int
main(void)
{
  int fd[2];
  int start = uptime() + 20;
  int response = 0, turnaround = 0;
  struct result r;
  if(pipe(fd) < 0)
    exit(1);
  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid < 0)
      exit(1);
    if(pid == 0){
      close(fd[0]);
      setpriority(i * 10);
      int remaining = start - uptime();
      if(remaining > 0)
        sleep(remaining);
      r.priority = getpriority();
      r.response = uptime() - start;
      // Fixed CPU work: every child performs the same number of additions.
      volatile uint sum = 0;
      for(uint j = 0; j < 1200000000; j++)
        sum += j;
      (void)sum;
      r.turnaround = uptime() - start;
      write(fd[1], &r, sizeof(r));
      close(fd[1]);
      exit(0);
    }
  }
  close(fd[1]);
  printf("priority response turnaround (ticks; completion order)\n");
  for(int i = 0; i < 3; i++){
    if(read(fd[0], &r, sizeof(r)) != sizeof(r))
      exit(1);
    printf("%d %d %d\n", r.priority, r.response, r.turnaround);
    response += r.response;
    turnaround += r.turnaround;
  }
  close(fd[0]);
  for(int i = 0; i < 3; i++)
    wait(0);
  printf("average response: %d.%d ticks\n", response / 3, response % 3 * 10 / 3);
  printf("average turnaround: %d.%d ticks\n", turnaround / 3, turnaround % 3 * 10 / 3);
  exit(0);
}
