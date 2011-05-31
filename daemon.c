#include <stdio.h>

#include <signal.h> // kill()

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define PID_MAXLEN 10 
#define PID_FILE "./daemon.pid"

/*
 * Make a daemon from current process.
 */
int daemonize() {
  pid_t pid, sid;
  FILE *fp ;
  char pid_str[PID_MAXLEN + 1];
    
  fp = fopen(PID_FILE, "r");

  if(fp != NULL) {
    fgets(pid_str, sizeof pid_str, fp);
    sscanf(pid_str, "%d", &pid);
    kill(pid, SIGKILL);
  }
  
  fclose(fp);
  
  // Spawn new process which will later act as daemon.
  pid = fork();
  
  if(pid < 0) {
    exit(EXIT_FAILURE);
  }
  
  // Close parent process in case daemon was spawned successfully.
  if(pid > 0) {
    exit(EXIT_SUCCESS);
  }
  
  if(pid == 0) {
    fp = fopen(PID_FILE, "w");
    printf("%s", PID_FILE);
    if(fp == NULL) {
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    
    sprintf(pid_str,"%d",getpid());
    fputs(pid_str, fp);
    fclose(fp);
  }
  
  // We have complete control over the permissions of 
  // anything we write. We don't know what umask we may have inherited.
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

main(int argc, char **argv)
{
  int pid = daemonize();
  
  while (1) {
    /* Do some task here ... */
    sleep(30); /* wait 30 seconds */
  }
}
