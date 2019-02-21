#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // int chdir(const char *path); 
#include <sys/types.h>
#include <sys/wait.h>

char *read_a_line(void);

char **parse_a_line(char *line);

int trident(char **args);