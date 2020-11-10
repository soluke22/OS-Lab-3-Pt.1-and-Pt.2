#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
  int pipefd[2];
  int pipe2[2];
  int pid;
  
  if(argc > 2){
    printf("Too many arguments, sorry!\n");
    return 0;
  }
  else if(argc <= 1){
    printf("Too little arguments, sorry!\n");
    return 0;
  }

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  pipe(pipe2);

  pid = fork();

  if (pid == 0)
    {

      dup2(pipefd[1], 1);

      close(pipefd[1]);
      close(pipefd[0]);
      close(pipe2[0]);
      close(pipe2[1]);

      execvp(*cat_args, cat_args);
      
    }
  else
    {
      if(fork()== 0){
        dup2(pipefd[0],0);
        dup2(pipe2[1],1);
          
        close(pipefd[1]);
        close(pipefd[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        
        execvp(*grep_args, grep_args);
        
      }
    else{
      if(fork() == 0)
      {
        dup2(pipe2[0],0);
        
        close(pipefd[1]);
        close(pipefd[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        
        execvp(*sort_args,sort_args); 
      } 
    }
      
  }
  
    close(pipefd[1]);
    close(pipefd[0]);
    close(pipe2[0]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
  
   
}