#include "kernel/types.h"
#include "user/user.h"

/*
 * Run as a prime-number processor
 * the listenfd is from your left neighbor
//  */
// void runprocess(int listenfd) {
//   int my_num = 0;
//   int forked = 0;
//   int passed_num = 0;
//   int pipes[2];
//   while (1) {
//     int read_bytes = read(listenfd, &passed_num, 4);

//     // left neighbor has no more number to provide
//     if (read_bytes == 0) {
//       close(listenfd);
//       if (forked) {
//         // tell my children I have no more number to offer
//         close(pipes[1]);
//         // wait my child termination
//         int child_pid;
//         wait(&child_pid);
//       }
//       exit(0);
//     }

//     // if my initial read
//     if (my_num == 0) {
//       my_num = passed_num;
//       printf("prime %d\n", my_num);
//     }

//     // not my prime multiple, pass along
//     if (passed_num % my_num != 0) {
//       if (!forked) {
//         pipe(pipes);
//         forked = 1;
//         // int ret = fork();
//         if (fork() == 0) {
//           // i am the child
//           close(pipes[1]);
//           close(listenfd);
//         //   runprocess(pipes[0]);
//           listenfd = pipes[0];
//           forked = 0;
//           my_num = 0;
//         } else {
//           // i am the parent
//           close(pipes[0]);
//         }
//       }

//       // pass the number to right neighbor
//       write(pipes[1], &passed_num, 4);
//     }
//   }
// }

// int main(int argc, char *argv[]) {
//   int pipes[2];
//   pipe(pipes);
//   for (int i = 2; i <= 35; i++) {
//     write(pipes[1], &i, 4);
//   }
//   close(pipes[1]);
//   runprocess(pipes[0]);
//   exit(0);
// }


int main(int argc, char *argv[]) {
    int me = 0, tmp = 0, forked=0;
    int read_pipe[2], write_pipe[2];
    pipe(read_pipe);
    for (int i = 2; i <= 35; i++)
        write(read_pipe[1], &i, 4);
    close(read_pipe[1]); 
    while (1) {
        if(read(read_pipe[0], &tmp, 4)){
            if (me == 0) {
                me = tmp;
                printf("prime %d\n", me);
            }
            if (tmp % me != 0) {
                if (!forked) {
                    pipe(write_pipe); // 初始化管道
                    forked = 1;
                    if (fork() == 0) { // 子进程重新进行初始化
                        close(write_pipe[1]);
                        close(read_pipe[0]);
                        read_pipe[0] = write_pipe[0];
                        forked = 0;
                        me = 0;
                    } else close(write_pipe[0]);
                }
                if (forked) write(write_pipe[1], &tmp, 4);
            }
        } else {
            close(read_pipe[0]);
            if (forked) {
                close(write_pipe[1]);
                int child_pid;
                wait(&child_pid);
            }
            exit(0);
        }
    }
}
