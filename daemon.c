#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h> 



int daemonize() {
  pid_t pid, sid;
  
  pid = fork();
  
  if(pid < 0) {
    exit(EXIT_FAILURE);
  }
  
  if(pid > 0) {
    exit(EXIT_SUCCESS);
  }
  
  umask(0);
  
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
  
  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }
  
//  close(STDIN_FILENO);
//  close(STDOUT_FILENO);
//  close(STDERR_FILENO);
  
  return getpid();
}

main()
{
  int pid = daemonize();
  
  printf("=> Process daemonized to %d\n", pid);
}
