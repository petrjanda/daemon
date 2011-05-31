#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define PID_MAXLEN 10 

int daemonize() {
  pid_t pid, sid;
  FILE *fp ;
  char pid_str[PID_MAXLEN + 1];
    
  printf("pid file path: %s \n", "/tmp/daemon.lock");

  //fgets(pid_str, sizeof pid_str, fp);
  
  // Spawn new process which will later act as daemon.
  pid = fork();
  
  if(pid < 0) {
    exit(EXIT_FAILURE);
  }
  
  // Close parent process in case daemon was spawned successfully.
  if(pid > 0) {
    fp = fopen("/tmp/daemon.lock", "w");
    
    if(fp == NULL)
    {
      printf("Cant open pid file!\n");
      exit(EXIT_FAILURE);
    }
    
    sprintf(pid_str,"%d",getpid());
    printf("pid=%s", pid_str);
    fputs(pid_str, fp);
    
    exit(EXIT_SUCCESS);
    
  }
  
  printf("Daemon pid %s written to lock file", pid_str);
  umask(0);
  
  // Create fresh session ID.
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
  
  // Change directory to system root.
  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }
  
  // Close stdout, stderr, stdin streams.
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  return getpid();
}

main()
{
  int pid = daemonize();
  
  printf("=> Process daemonized to %d\n", pid);
}
