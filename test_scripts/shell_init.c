/**
 * Compiler Notes: including readline lib 
 * gcc code.c -L/usr/local/lib -I/usr/local/include -lreadline
 *
 */

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // int chdir(const char *path); 
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h> 

// Clear the shell using escape 
#define clear() printf("\033[H\033[J")
// hold the current line being processed 
static char *line = (char *)NULL; 
#define maxletters 1000
#define maxcommands 100

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
    printf("|------------------------------|\n");
    printf("|                              |\n"); 
    printf("|---  >> man to open manual ---|\n");
    printf("|                              |\n"); 
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

void print_man() {
    printf("\n------ Laf Shell Manual ------\n"); 
    printf("--- List of Supported Cmds ---\n\n"); 
    printf("lafsh>> exit\n");
    printf("lafsh>> cd\n"); 
    printf("lafsh>> man\n"); 
    printf("lafsh>> clear\n"); 
    printf("lafsh>> ls\n"); 
    printf("\n**Pipe Handling Also Supported**\n");
    printf("----------------------------------"); 
    return;
}

void parse_out_spaces(char* str, char** parsed) {

    for (int i = 0; i < 100; i ++) {
        // find spaces 
        parsed[i] = strsep(&str, " "); 
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i --; 
    }
}

// handle pipes here later 
int proccess_cmd(char* str, char** parsed) {
    parse_out_spaces(str, parsed); 
    int temp = -1; 
    temp = set_cmds_handler(parsed); 
    if (temp)
        return 0; 
    else 
        return 1; 
}

int set_cmds_handler(char** curr_cmd) {
    int numcommands = 5; 
    int switch_case_arg = 0; 
    char* listofcommands[numcommands];

    listofcommands[0] = "exit"; 
    listofcommands[1] = "cd"; 
    listofcommands[2] = "man"; 
    listofcommands[3] = "clear"; 
    listofcommands[4] = "ls";
    
    for (int i = 0; i < numcommands; i ++) {
        // compare command typed to list of preset commands 
        if (strcmp(curr_cmd[0], listofcommands[i]) == 0) {
            switch_case_arg = i + 1; // increase comparison in array
            break; 
        }

        switch (switch_case_arg) {
        // case 1 is 'exit' so exit the shell 
        case 1:
            printf("\nExiting Laf Shell...\n"); 
            exit(0);
        
        // case 2 is 'cd' which means return to home dir     
        case 2: 
            // chdir() changes current working directory 
            // used to implement 'cd' command in shell sim 
            chdir(curr_cmd[1]);
            return 1; // success! 
        
        // case 3 is 'man' --> user wants to see manual for shell 
        case 3:
            print_man(); 
            return 1; 
        
        // case 4 is 'clear' --> clear the screen 
        case 4:
            clear();
            return 1; 

        // case 5 is 'ls' --> display contents of cwd 
        case 5:
            return 1;

        default:
            break; 
        }
    }
    return 0;
}

// NOTE I ALSO WANT TO  IMPLEMENT printenv FUNCTIONALITY IT'S COOL 
void set_env_var(char* newvar) {

}

void rm_env_var(char* oldchar) {

}

void execute_sys_command(char** temp_parsed) {
    // This is not a fork, it's a trident 
    // Tridents are for ruling the sea 
    pid_t pid = fork(); 

    if (pid == -1) {
        // failed 
        printf("\nFailed Fork- Maybe it's a Spoon"); 
        return;
    } else if (pid == 0) {
        if (execvp(temp_parsed[0], temp_parsed) < 0) {
            printf("\nError Executing Command"); 
        }
        exit(0); 
    } else {
        // wait for child to terminate sequence 
        wait(NULL);
        return; 
    }
}

int main() {
    char inputString[maxletters];
    char *parsed[maxcommands];
    //char *parsedPipes[maxcommands]; 
    int pipe_flag = 0; 

    init_shell(); 
    while (1) {
        // Make sure the current line is 
        if (process_input_line(inputString))
        continue; 

        pipe_flag = proccess_cmd(inputString, parsed); 
        // if pipe_flag is 0, it's a built in command and will be executed
        // if pipe_flag is 1, then go to sys command 
        if (pipe_flag == 1) 
            // execute arg(parsed) 
            execute_sys_command(parsed); 
    }
    return 0;
}