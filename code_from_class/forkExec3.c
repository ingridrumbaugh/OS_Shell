#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  pid_t pid[2];
  char *argv2[2][3];
  argv2[0][0] = argv2[1][0] = "/home/ordillej/examples/sysServicesIntro/giveme";
  argv2[0][1] = "pizza";
  argv2[1][1] = "cookies";
  argv2[0][2] = argv2[1][2] = NULL;

  for (int i = 0; i<2; i++) {
    pid[i] = fork();
    if (pid < 0) {
      fprintf(stderr,"%s fork error creating child %d: %s\n", argv[0], i, strerror(errno));
      exit(1);
    }
    if (pid[i] == 0) {  /* Child */
      execv(argv2[i][0], argv2[i]);
    }
    printf("Child %d created\n",pid[i]);
    int status;
    pid_t childDone = wait(&status);
    if (WIFEXITED(status)) {
      printf("Child %d exited successfully\n", childDone);
    } else {
      printf("Child %d exited after signal\n", childDone);
    }
}

/* Parent */
   printf("Parent completing successfully after reaping children %d and %d.\n", \
       pid[0], pid[1]);
   exit(0);
}