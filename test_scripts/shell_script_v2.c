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

static volatile int keepRunning = 1; 
 
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

int fork_pipes2(char *args[]) {
    pid_t pid1;
    int pipefd[2]; 
    int status; 
    // create a pipe 
    pipe(pipefd); 
    // create 1st process
    pid1 = fork(); 
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); 
        fprintf(stderr, "About to execute\n");
        execvp(args[0], args);
        perror("exec");
        return 1; 
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, &status, 0); 
    return 0;
}

int parse_pipe(char *temp_line) {
    fprintf(stderr, "Parsing Pipe Cmd\n");
    char delim1[] = " |"; 
    char delim2[] = "|";
    int iter;
    char * line_nospaces;
    char * line_args;
    char * arg1;
    char * arg2;
    char * arg3;
    char * arg4; 
    char * all_args[4];

    line_nospaces = strtok(temp_line, delim1); 
    arg1 = line_nospaces; 
    fprintf(stderr,"Arg1 = %s\n\n",arg1);
    
    iter = 0;
    // while (line_nospaces != NULL) {
        line_nospaces = strtok(NULL, delim1); 
        arg2 = line_nospaces; 
        line_nospaces = strtok(NULL, delim1); 
        arg3 = line_nospaces; 
        line_nospaces = strtok(NULL, delim1); 
        arg4 = line_nospaces; 
        fprintf(stderr,"Arg2 = %s\n\n",arg2);
        fprintf(stderr,"Arg3 = %s\n\n",arg3);
        fprintf(stderr,"Arg4 = %s\n\n",arg4); 
    // } 
    // iter = 0;
    // for (line_args = strtok(line_nospaces, delim2); line_args != NULL; line_args = strtok(NULL, delim2)) {
    //     if (iter == 0) {
    //         arg1 = line_args; 
    //         fprintf(stderr, "Arg 1 = %s\n",arg1);
    //         //strcpy(arg1, line_args); 
    //     }
    //     else if (iter == 1) {
    //         arg2 = line_args;
    //         fprintf(stderr, "Arg 2 = %s\n",arg2);
    //         //strcpy(arg2, line_args); 
    //     }
    //     else if (iter == 2) {
    //         arg3 = line_args; 
    //         fprintf(stderr, "Arg 3 = %s\n",arg3);
    //         //strcpy(arg3, line_args); 
    //     }
    //     else if (iter == 3) {
    //         arg4 = line_args; 
    //         fprintf(stderr, "Arg 4 = %s\n",arg4);
    //         //strcpy(arg4, line_args); 
    //     }
    //     iter ++;
    // }
    if (arg1 != NULL)
        all_args[0] = arg1; 
    if (arg2 != NULL)
        all_args[1] = arg2;
    if (arg3 != NULL)
        all_args[2] = arg3; 
    if (arg4 != NULL)
        all_args[3] = arg4; 
    //fprintf(stderr,"Line_NoSpaces = %s\n\n",line_nospaces);
    fprintf(stderr,"Calling fork_pipes2 now\n"); 
    fork_pipes2(all_args);
    return 0;
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
        parse_pipe(line);
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

void signalHandler(int signo) {
    if (signo == SIGINT) {
        fprintf(stderr, "received SIGINT\n");
        keepRunning = 0; 
    }
}

int main(int argc, char **argv) {
    char *line;
    char **args;
    int status;
    int pipe_flag = 0;

    init_shell(); 
    signal(SIGINT, signalHandler);  
    while (keepRunning) {
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

    }
    return EXIT_SUCCESS;
}
