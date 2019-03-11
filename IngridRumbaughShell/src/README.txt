Ingrid Rumbaugh, Computer Science Dept Lafayette College, 2019

LAFAYETTE SHELL
Mini C Shell to Emulate the Linux Command Line.
The goal of this project is to increase understanding of the Unix programming
environment, especially shells. 
This program creates, monitors, and reports on process in Linux.  
It also creates pipelines, as well as incorporates environment variables into
the functionality. 

MOTIVATION 
This was a short project for CS 406 at Lafayette College in order to familiarize
students with C programming and writing shells. 

BUILD STATUS 
Built status of this project is complete. 

CODE STYLE 
standard, C 

LIBRARIES USED 
<stdio.h>
<string.h>
<stdlib.h>
<unistd.h>
<sys/types.h>
<sys/wait.h>
<sys/stat.h>
<signal.h>
<fcntl.h>

FEATURES
Supports pipes (>> ls -l | sort | uniq) 
Supports environment variables (set, unset) 
Supports >> echo $PATH 

INSTALLATION AND USE 
1. un-tar the IngridRumbaughShell folder in a convenient directory 
2. Read the Manual located in IngridRumbaughShell/install/man
3. Navigate to IngridRumbaughShell/install/bin
4. The executable for this project is entitled "shell_script_v2"
5. To run the shell, type ./shell_script_v2 into the Linux Terminal 

CREDITS 
Ingrid Rumbaugh under the guidance of Joann Ordille, PhD

LICENSE 
GPLv3.0 (c) Ingrid Rumbaugh 


