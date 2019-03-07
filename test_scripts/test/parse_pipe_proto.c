int parse_pipe(char *temp_line, char *delim1) {
    fprintf(stderr, "Parsing Pipe Cmd\n");
    // char delim1[] = " "; 
    // char delim2[] = "|";
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
    // line_nospaces = strtok(NULL, delim1); 
    // arg3 = line_nospaces; 
    // line_nospaces = strtok(NULL, delim1); 
    // arg4 = line_nospaces; 
    fprintf(stderr,"Arg2 = %s\n\n",arg2);
        
    if (arg1 != NULL)
        all_args[0] = arg1; 
    if (arg2 != NULL)
        all_args[1] = arg2;

    all_args[2] = NULL;
    // if (arg3 != NULL)
    //     all_args[2] = arg3; 
    // if (arg4 != NULL)
    //     all_args[3] = arg4; 
    //fprintf(stderr,"Line_NoSpaces = %s\n\n",line_nospaces);
    fprintf(stderr,"Calling fork_pipes2 now\n"); 
    fork_pipes2(all_args);
    return 0;
}
