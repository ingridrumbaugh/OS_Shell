int set_cmds_handler(char** curr_cmd) {
        int numcommands = 3; 
        int switch_case_arg = 0; 
        char* listofcommands[numcommands]; 

        listofcommands[0] = "quit";
        listofcommands[1] = "help"; 
        listofcommands[2] = "cd";

        for (int i = 0; i < numcommands; i ++) {
            // compare command typed to list of preset commands 
            if (strcmp(curr_cmd[0], listofcommands[i]) == 0) {
                switch_case_arg = i + 1; // inc comparison in array
                break; 
            }

            switch (switch_case_arg) {
                // case 1 is 'quit' 
            case 1:
                printf("\nExiting Laf Shell...\n"); 
                exit(0); 
                // case 2 is 'help'
            case 2:
                print_man();
                return 1; 

            default:
                break;
            }
        }
        return 0; 
    }
