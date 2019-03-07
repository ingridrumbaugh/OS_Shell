
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



// int fork_pipes2(char *args[]) {
//     pid_t firstpid, lastpid, middlepid;
//     int pipefd[2]; // file descriptor exit1 --> entrance2  
//     int pipefd2[2];
//     int status, i;

//     for (i = 0; args[i] != NULL; i ++) {
//         fprintf(stderr,"Arg %d is %s\n",i,args[i]);
//     }
//     // create a pipe 
//     pipe(pipefd); 

//     // create 1st process
//     firstpid = fork(); 
//     if (firstpid == 0) { // FIRST ARG 
//         current_pid = firstpid; 
//         dup2(pipefd[1], STDOUT_FILENO);
//         close(pipefd[0]); 
//         close(pipefd[1]);
//         fprintf(stderr, "About to execute\n");
//         char ** args2 = parse_a_line(args[0]);
//         execvp(args2[0], args2);
//         perror("exec");
//         return 1; 
//     }

//     // BOTH INPUT AND OUTPUT IN MIDDLE ARGS 
//     // pipefd[0], pipefd2[1] is output 
//     // If there are more than 2 args, then create middle pipe 

//     pipe(pipefd2); 
//     int index = 2; 
//     char *current = args[index]; 
//     middlepid = fork(); 
//     current_pid = middlepid; 
    
//     if (middlepid == 0) {
//         // set up 1st pipe to be read from 
//         // close unneeded pipe endpoints 
//         //close(pipefd[1]); 
//         dup2(pipefd[0], 0);
//         dup2(pipefd2[1], 1); 

//         // set up 2nd pipe to be written 
//         // close unneeded pipe endpoints 
//         // hi don't make an infinite loop 
//         close(pipefd[0]);
//         close(pipefd[1]); 
//         close(pipefd2[0]);
//         close(pipefd2[1]); 
//         char ** args3 = parse_a_line(args[0]); 
//         execvp(args3[0], args3); 
//         perror("middle exec"); 
//         return 1;
//     }
//     close(pipefd[1]);
//     close(pipefd[0]); 
        
//     //     if (middlepid == 0) {
//     //         current_pid = middlepid; 
//     //         dup2(pipefd[0], pipefd2[1]); 
//     //         close(pipefd[0]);
//     //         close(pipefd[0]);
//     //         close(pipefd2[1]); 
//     //             //fprintf(stderr, "Inside middlepid loop\n"); 
//     //         char ** args3 = parse_a_line(args[index]); 
//     //         execvp(args3[0], args3);
//     //         index ++;
//     //         perror("middle exec");
//     //         return 1; 
//     //     }

//     lastpid = fork();
//     if (lastpid == 0) { // LAST ARG 
//         current_pid = lastpid; // keep track of fg job 
//         dup2(pipefd[0], STDIN_FILENO);
//         close(pipefd[0]); 
//         close(pipefd[1]);
//         fprintf(stderr, "About to execute\n");
//         // i is the size of args and will point to last item in args
//         char ** args2 = parse_a_line(args[1]);
//         execvp(args2[0], args2);
//         perror("exec");
//         return 1; 
//     }
//     close(pipefd[0]);
//     close(pipefd[1]);

//     waitpid(firstpid, &status, 0); 
//     return 0;
// }
