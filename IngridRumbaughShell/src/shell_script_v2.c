#include "shell_script_v2.h" 

// define buffers for writing things to memory 
#define linebuffersize 1024; 
#define tokenbuffersize 64; 
#define tokendelims " \t\r\n\a"
#define clear() printf("\033[H\033[J") 

// used for the catching the ^C signal in signal handler 
static volatile int keepRunning = 1; 

// function declarations for builtins
int cd(char **args);
int help(char **args); 
int quit(char **args);
int echo_path();

// this is a global pid that will point to current process
pid_t current_pid; 

// this is what is printed when the user starts the shell 
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

// this is what appears when the user types >> help 
// directs the user to the full manual and provides add'l info 
void print_man() {
    printf("\n------ Laf Shell Manual ------\n"); 
    printf("--- List of Supported Cmds ---\n\n"); 
    printf("Man Page /IngridRumbaughShell/src/man\n");
    printf("lafsh>> quit\n");
    // translate (fork) to man -l nameofmanfile.txt
    printf("lafsh>> help\n"); 
    printf("lafsh>> cd\n");
    printf("lafsh>> echo $PATH"); 
    printf("\n**Pipe Handling Also Supported**\n");
    printf("----------------------------------\n"); 
    return;
}

// these are the list of builtin commands 
char *listofcommands[] = {
    "quit",
    "help",
    "cd",
    "$PATH"
};

// these are the addresses of the 1st element of array for builtins
int (*builtin_func[]) (char**) = {
    &quit,
    &help,
    &cd,
    &echo_path
};

int get_num_commands() {
    return sizeof(listofcommands) / sizeof(char *); 
}

// define a builtin command 
// change working directory with >> cd command 
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

// define a builtin command 
// display additional info when user types help cmd 
int help(char **args) {
    print_man(); 
    // need to print man.txt fork to location 
    return 1;
}

// quit the shell 
int quit(char **args) {
    exit(2);
    return 0; 
}

// print the current path using getenv()
int echo_path() {
    printf("PATH: %s\n", getenv("PATH")); 
}

// set a new environment variable 
// if it already exists, unset it 
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
    pid_t pid[3];
    int n;
    int ab[2], bc[2], ca[2];

    pid[0] = fork();
    if(pid[0] == 0) {
        // Process A reads from process C
        dup2(ca[0], STDIN_FILENO);
        // Process A writes to process B
        dup2(ab[1], STDOUT_FILENO);
        // Essential: close all other pipes and copies of pipes
        for(n = 0; n < 2; n++) { 
            close(ab[n]); 
            close(bc[n]); 
            close(ca[n]); 
        }

        char ** args2 = parse_a_line(args[0]);
        execvp(args2[0], args2);
        perror("exec");
        //return 1; 
        // quit so your child doesn't end up in the main program
        exit(0);
    }

    pid[1] = fork();
    if(pid[1] == 0) {
        // Process B reads from process A
        dup2(ab[0], STDIN_FILENO);
        // Process B writes to process C
        dup2(bc[1], STDOUT_FILENO);
        // Essential: close all other pipes and copies of pipes
        for(n = 0; n < 2; n++) { 
            close(ab[n]); 
            close(bc[n]); 
            close(ca[n]); 
        }

        char ** args3 = parse_a_line(args[0]);
        execvp(args3[0], args3);
        // quit so your child doesn't end up in the main program
        exit(0);
    }

    pid[2] = fork();
    if(pid[2] == 0) {
        // Process C reads from process B
        dup2(bc[0], STDIN_FILENO);
        // Process C writes to process A
        dup2(ca[1], STDOUT_FILENO);
        // Essential: close all other pipes and copies of pipes
        for(n= 0; n < 2; n++) { 
            close(ab[n]); 
            close(bc[n]); 
            close(ca[n]); 
        }

        char ** args1 = parse_a_line(args[0]);
        execvp(args1[0], args1);
        // quit so your child doesn't end up in the main program
        exit(0);
    }

// Only the parent will be running outside of those if statements.
// Essential: close all other pipes and copies of pipes
    for(n = 0; n < 2; n++) { 
        close(ab[n]); 
        close(bc[n]); 
        close(ca[n]); 
    }

    for(n = 0; n < 3; n++) {
        int status;
        waitpid(pid[n], &status, 0);
        printf("Child %d exited with status %d\n", n, WEXITSTATUS(status));
    }
}

int parse_pipe(char *line, char *delim1) {

    fprintf(stderr, "Parsing Pipe Cmd\n");
    int buffer_size = tokenbuffersize;
    char **tokens = malloc(buffer_size *sizeof(char*)); 
    int line_position = 0;  
    char *token;

    if (!tokens) {
        fprintf(stderr, "Memory Allocation Error\n"); 
        exit(EXIT_FAILURE); 
    }
    
        token = strtok(line, delim1);
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
            token = strtok(NULL, delim1); 
        }
    
    tokens[line_position] = NULL;
    fork_pipes2(tokens);
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
        parse_pipe(line, "|");
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
        // check if the line contains a builtin command 
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
        current_pid = pid;
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
        // kill it using global pid 
        kill(current_pid, SIGKILL); 
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
