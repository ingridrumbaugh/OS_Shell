#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat> 
#include <signal.h>
#include <fcntl.h>


/**
 * @brief: clears the screen in the terminal and then prints the 
 *         opening screen that displays the name, author, and help
 *         command. 
 **/
void init_shell(); 

/**
 * @brief: this is the function that is called when the user types in
 *         >> help. It displays the supported commands and tells the 
 *         user where to find the full man page. 
 *
 **/
void print_man();

/**
 * @brief: Defines the list of preset commands in the shell 
 *         These are: quit, help, cd, and echo $PATH 
 * @param: Returns an array of type char * listing the preset 
 *         commands 
 *
 **/
char *listofcommands[];

/**
 * @brief: Used to define the addresses of the builtin commands
 *         This command is used for determining if the user has
 *         typed in a preset command
 * @param: returns address of specified builtin command 
 *
 **/
int (*builtin_func[]) (char**);

/**
 * @brief:
 * @param: 
 *
 **/
int get_num_commands();

/**
 *
 *
 **/
int echo_path();

/**
 *
 *
 **/
void set_env(char *name, char *val);

/**
 *
 *
 **/
char *read_a_line(void);

char *parse_equals(char *temp_arg);

int fork_pipes2(char *args[]);

char *parse_pipe(char *temp_line);

char **parse_a_line(char *line);

int trident(char **args);

int launch_shell(char **args);

void signalHandler(int signo); 

