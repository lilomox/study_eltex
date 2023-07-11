#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t child_pid;
  int status;

  child_pid = fork();

  if (child_pid == 0) {
    printf("Child pid = %d\n", getpid());
    exit(5);

  } else {
    printf("Parent pid = %d\n", getpid());
    wait(&status);
    printf("Status = %d\n", WEXITSTATUS(status));
  }

  return 0;
}
