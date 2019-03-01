if (fork() == 0)
    {
      // replace cat's stdout with write part of 1st pipe

      dup2(pipes[1], 1);

      // close all pipes (very important!); end we're using was safely copied

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      execvp(*cat_args, cat_args);
    }
  else
    {
      // fork second child (to execute grep)

      if (fork() == 0)
    {
      // replace grep's stdin with read end of 1st pipe
      
      dup2(pipes[0], 0);

      // replace grep's stdout with write end of 2nd pipe

      dup2(pipes[3], 1);

      // close all ends of pipes

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      execvp(*grep_args, grep_args);
    }
      else
    {
      // fork third child (to execute cut)

      if (fork() == 0)
        {
          // replace cut's stdin with input read of 2nd pipe

          dup2(pipes[2], 0);

          // close all ends of pipes

          close(pipes[0]);
          close(pipes[1]);
          close(pipes[2]);
          close(pipes[3]);

          execvp(*cut_args, cut_args);
        }
    }
    }
      
  // only the parent gets here and waits for 3 children to finish
  
  close(pipes[0]);
  close(pipes[1]);
  close(pipes[2]);
  close(pipes[3]);

  for (i = 0; i < 3; i++)
    wait(&status);
}