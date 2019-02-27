#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // int chdir(const char *path); 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char *read_a_line(void);

char **parse_a_line(char *line);

int trident(char **args);

void init_shell(); 

void print_man();

int set_cmds_handler(char** curr_cmd);

char *listofcommands[];

int (*builtin_func[]) (char**);

int get_num_commands();

int launch_shell(char **args);

int echo_path();

void set_env(char *name, char *val);

char *parse_equals(char *temp_arg);