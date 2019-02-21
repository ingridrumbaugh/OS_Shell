// ADD EXIT() FUNCTION 
// ADD MAN PAGE PRINT FUNCTION AND START PAGE 
// ADD $PATH to echo path env var 
// ADD ENVIRONMENT VAR FUNCTIONALITY 

// ADD PIPE SUPPORT FOR SHELL 
// SUPPORT REQS OF HW PROBLEM 8.26 IN COMP SYS TEXTBOOK 

#include "read_a_line.h" 

#define linebuffersize 1024; 
#define tokenbuffersize 64; 
#define tokendelims " \t\r\n\a"

char *read_a_line(void) {
    char *current_line = NULL; 
    // used to represent sz of allocated block of memory
    ssize_t buffer_size = 0;
    getline(&current_line, &buffer_size, stdin);
    return current_line;  
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
    tokens[line_position] = NULL;
    return tokens;
}

int trident(char **args) {
    pid_t pid, wpid; 
    int status; 

    pid = fork(); 
    if (pid == 0) {
        // start child process 
        if (execvp(args[0], args) == -1) {
            perror("lafshell>");
        }
        exit(EXIT_FAILURE); 
    } else if (pid < 0) {
        perror("lafshell>"); 
    } else {
        // this is the parent 
        do {
            wpid = waitpid(pid, &status, WUNTRACED); 
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1; 
}

int main(int argc, char **argv) {
    char *line;
    char **args;
    int status; 

    do {
        printf("lafshell> "); 
        line = read_a_line(); 
        args = parse_a_line(line); 
        status = trident(args); 

        free(line);
        free(args); 
    } while (status); 

    return EXIT_SUCCESS;
}
