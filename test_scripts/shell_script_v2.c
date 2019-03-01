// ADD MAN PAGE PRINT FUNCTION AND START PAGE 
// ADD ENVIRONMENT VAR FUNCTIONALITY 
//int setenv(const char *name, const char *value, int overwrite);
//int unsetenv(const char *name);

// ADD PIPE SUPPORT FOR SHELL 
// SUPPORT REQS OF HW PROBLEM 8.26 IN COMP SYS TEXTBOOK 

#include "read_a_line.h" 

#define linebuffersize 1024; 
#define tokenbuffersize 64; 
#define tokendelims " \t\r\n\a"
#define clear() printf("\033[H\033[J") 
 
// function declarations for builtins
int cd(char **args);
int help(char **args); 
int quit(char **args);
int echo_path();

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
    printf("|--- >> help for shell help ---|\n");
    printf("|                              |\n"); 
    printf("|------------------------------|\n\n\n");
}

void print_man() {
    printf("\n------ Laf Shell Manual ------\n"); 
    printf("--- List of Supported Cmds ---\n\n"); 
    printf("lafsh>> quit\n");
    // translate (fork) to man -l nameofmanfile.txt
    printf("lafsh>> help\n"); 
    printf("lafsh>> cd\n");
    printf("lafsh>> echo $PATH"); 
    printf("\n**Pipe Handling Also Supported**\n");
    printf("----------------------------------\n"); 
    return;
}

char *listofcommands[] = {
    "quit",
    "help",
    "cd",
    "$PATH"
};

int (*builtin_func[]) (char**) = {
    &quit,
    &help,
    &cd,
    &echo_path
};

int get_num_commands() {
    return sizeof(listofcommands) / sizeof(char *); 
}

int cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Laf Shell: Expected Arg to \"cd\"\n"); 
    } else {
        if (chdir(args[1]) != 0) {
            perror("lafsh"); 
        }
    }
    return 1;
}

int help(char **args) {
    print_man(); 
    // need to print man.txt fork to location 
    return 1;
}

int quit(char **args) {
    return 0; 
}

int echo_path() {
    printf("PATH: %s\n", getenv("PATH")); 
}

void set_env(char *name, char *val) {
    if (val[0] == 0) {
        unsetenv(name); 
    } else {
        setenv(name, val, 1);
        printf("Setting env var: %s\n", name); 
        printf("equals: %s\n", val);
    }  
}

char *read_a_line(void) {
    char *current_line = NULL; 
    // used to represent sz of allocated block of memory
    ssize_t buffer_size = 0;
    getline(&current_line, &buffer_size, stdin);
    return current_line;  
}

char *parse_equals(char *temp_arg) {
    char *var_name;
    char *var_path; 
    char delim[] = "="; 
    char *token = strtok(temp_arg, delim); 
    // check if this env var 
    var_name = token; 
    token = strtok(NULL, delim); 
    var_path = token;

    while(*token != 0) {
        if (*token == '\n') {
            //fprintf(stderr, "found new line!!!\n");
            *token = 0;
        }
        token ++;
    }
    set_env(var_name, var_path); 
}

char **parse_a_line(char *line) {
    int buffer_size = tokenbuffersize;
    char **tokens = malloc(buffer_size *sizeof(char*)); 
    int line_position = 0;  
    char *token;

    if (!tokens) {
        fprintf(stderr, "Memory Allocation Error\n"); 
        exit(EXIT_FAILURE); 
    }
    // the line is trying to set an env var 
    if (strstr(line, "=") != NULL) {
        parse_equals(line);
 
    } else if (strstr(line, "|") != NULL) {
        // cal fn to parse out ' ' space 
        // add each arg to array args[] 
        // execute 
    } else {
        token = strtok(line, tokendelims);
        while (token != NULL) {
            tokens[line_position] = token;
            line_position ++; 
            // if we've exceeded the buffer, 
            if (line_position >= buffer_size) {
                buffer_size += tokenbuffersize;
                tokens = realloc(tokens, buffer_size *sizeof(char*)); 
                if (!tokens) {
                    fprintf(stderr, "Memmory Allocation Error\n"); 
                    exit(EXIT_FAILURE); 
                }
            }
            token = strtok(NULL, tokendelims); 
        }
    }
    tokens[line_position] = NULL;
    return tokens; 
}

int trident(char **args) {
    int i; 
    // if empty command entered, return 
    if (args[0] == NULL) {
        return 1; 
    }
    for (i = 0; i < get_num_commands(); i ++) {
        
        if (strcmp(args[0], listofcommands[i]) == 0) { 
            return (*builtin_func[i])(args);
        } else if (args[1] != NULL) {
            if (strcmp(args[1], listofcommands[i]) == 0) {
                return (*builtin_func[i])(args); 
            }
        }
    }
    return launch_shell(args); 
}

int launch_shell(char **args) {
    pid_t pid, wpid;
    int status; 

    pid = fork(); 
    if (pid == 0) {
        // start child process 
        if (execvp(args[0], args) == -1) {
            perror("Error Occurred");
        }
        exit(EXIT_FAILURE); 
    } else if (pid < 0) {
        perror("Error Occurred"); 
    } else {
        // this is the parent 
        do {
            wpid = waitpid(pid, &status, WUNTRACED); 
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1; 
}


int fork_pipes(char **args) {
    int pipes[4]; 
    pipe(pipes); // set up 1st pipe
    pipe(pipes+2); 

    if (fork() == 0) {
        dup2(pipes[1], 1); 

        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);

        execvp(args[0], args); 
    } else {
        // fork 2nd child 
        if (fork() == 0) {
            dup2(pipes[0], 0); 
            dup2(pipes[3], 1); 

            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);

            execvp(args[0], args); 
        } else {
        // fork 3rd child if exists 
            if (fork() == 0) {
                dup2(pipes[2], 0); 

                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);

                execvp(args[0], args);
            }
        } 
    }
    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);
}

int main(int argc, char **argv) {
    char *line;
    char **args;
    int status;
    int pipe_flag = 0;

    init_shell();  

    do {
        printf("lafshell> "); 
        line = read_a_line(); 
        args = parse_a_line(line); 

        if (args[0] != NULL) {
            status = trident(args); 
        }
        
        free(line);
        free(args); 
    } while (status); 

    return EXIT_SUCCESS;
}
