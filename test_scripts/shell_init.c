#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h> 

// Clear the shell using escape 
#define clear() printf("\033[H\033[J")
// hold the current line being processed 
static char *line = (char *)NULL; 
int numcommands = 5; 

void set_list_cmds() {
    char* listofcommands[numcommands];
    listofcommands[0] = "exit"; 
    listofcommands[1] = "cd"; 
    listofcommands[2] = "man"; 
    listofcommands[3] = "clear"; 
    listofcommands[4] = "ls";
}

void init_shell() {
    clear();
    printf("\n\n\n|------------------------------|");
    printf("\n|                              |"); 
    printf("\n|                              |\n"); 
    printf("|-------Lafayette Shell--------|");
    printf("\n|                              |"); 
    printf("\n|-- CS 406   Ingrid Rumbaugh --|\n");
    printf("|                              |"); 
    printf("\n|                              |\n"); 
    printf("|------------------------------|\n\n\n");
}

// I decided to use the GNU Readline Library
// Example code I read for using this lib: 
// http://web.mit.edu/gnu/doc/html/rlman_2.html
int process_input_line(char* str) {
    
    char* curr_line; 

    curr_line = readline("\nlafsh>> "); // read a line 
    // if line has text in it, save it to history 
    if (curr_line && *curr_line) {
        add_history(curr_line); 
        line = curr_line;
        return 0;
    } else {
        return 1; 
    }
}

// NOTE I ALSO WANT TO  IMPLEMENT printenv FUNCTIONALITY IT'S COOL 
void set_env_var(char* newvar) {

}

void rm_env_var(char* oldchar) {

}

int set_cmds_handler(char** curr_cmd) {
    return 0;
}

int main() {
    init_shell(); 
    set_list_cmds(); // just set the preset commands, doesn't print anything
    process_input_line(line); 
    return 0;
}