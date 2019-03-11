#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
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
 * @brief: Returns the number of 'preset' commands by returning the 
 *         size of the listofcommands function. 
 *
 **/
int get_num_commands();

/**
 * @brief: Returns the current path var
 *         using the getenv() function. 
 *         returns 1 for success and not 1 for error 
 *
 **/
int echo_path();

/**
 * @brief: Sets a new environment variable 
 *         Accepts the name and "value" of the new variable 
 *         Uses the unsetenv() fn if the variable already exists
 *         Uses setenv() fn if the variable doesn't already exists 
 *
 **/
void set_env(char *name, char *val);

/**
 * @brief: Reads the current line from the command line 
 * @param: Returns the current line as a char * 
 *
 **/
char *read_a_line(void);

/**
 * @brief: Used when setting environment vars 
 *         Parses the "=" out of the line from cmd line so 
 *         var name and var value can be separated 
 * @param: temp_arg is the current cmd line 
 *
 **/
char *parse_equals(char *temp_arg);

/**
 * @brief: 2nd version of fork_pipes 
 *         This function is called instead of trident when the 
 *         program detects that there is a pipe instead of 1 arg
 *
 * Contains 3 IF blocks:
 * 1 for the 1st argument where the output of 1 is input of 2nd
 * 1 for the Middle arg(s) where 2nd reads from 1st, 2nd writes to 3rd
 * 1 for the last argument where the 3rd reads from 2nd and 3rd writes to 1st
 * 
 * @param: *args[] is the tokenized current line 
 *         This happens in parse_pipe function 
 *
 **/
int fork_pipes2(char *args[]);

/**
 * @brief: Used when the program evaluates the current line and detects
 *         that there is a "|" used. This function parses out the line
 *         in the same way that parse_a_line does
 * @param: *temp_line is the line to be parsed 
 *         *delim1 is the delimiter to be used ("|")
 *
 **/
int parse_pipe(char *temp_line, char *delim1);

/**
 * @brief: Parses the line from cmd line that user types 
 *         Forks the program off to another function if the line contains
 *         a = or | (setenv or a pipe)
 *         Otherwise it tokenizes the string 
 * @param: *line is arg taken - this is the current line being parsed
 *         returns the token array (char**)
 *
 **/
char **parse_a_line(char *line);

/**
 * @brief: Takes a char* array and returns if NULL, 
 *         compares it to a list of builtin functions, if it matches,
 *         it'll execute that. If not, launch_shell is called 
 * @param:  **args is the tokenized array of the current line 
 *
 **/
int trident(char **args);

/**
 * @brief: This function sets the pid, wpid, and status as well as 
 *         fork() the current process to start the child process
 * @param: **args is the array of arguments contained in the current line 
 *
 **/
int launch_shell(char **args);

/**
 * @brief: Currently only catches a ^C signal from user
 *         If there is a program running in the foreground it will
 *         be killed using SIGKILL and the current pid 
 *         If not, then the shell will continue 
 * @param: signo is the signal number set to SIGINT
 *
 **/
void signalHandler(int signo); 

