#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv){
  fprintf(2, "uptime: %d\n", uptime());
  exit(0);
}