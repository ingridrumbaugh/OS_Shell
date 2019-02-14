#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2) { 
      fprintf(stderr, "usage: %s <string>\n", argv[0]);
      exit(1); 
    }

    while (1) {
      printf("I want %s!\n", argv[1]);
      fflush(stdout);
      sleep(1);
    }

    return 0;
}
