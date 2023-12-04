#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pid, p1[2], p2[2];
  char buf[] = {'?'};
  pipe(p1);
  pipe(p2);

  int ret = fork();

  // parent send in p1[1], child receives in p1[0]
  // child send in p2[1], parent receives in p2[0]
  // should have checked close & read & write return value for error, but i am
  // lazy
  if (ret == 0) {
    // i am the child
    pid = getpid();
    // close(p1[1]);
    // close(p2[0]);
    read(p1[0], buf, 1);
    printf("%d: received ping\n", pid);
    write(p2[1], buf, 1);
    exit(0);
  } else {
    // i am the parent
    pid = getpid();
    // close(p1[0]);
    // close(p2[1]);
    write(p1[1], buf, 1);
    read(p2[0], buf, 1);
    printf("%d: received pong\n", pid);
    exit(0);
  }
}