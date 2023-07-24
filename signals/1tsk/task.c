#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("Program 1 started.\n");

  pid_t child_pid = fork();

  if (child_pid < 0) {
    perror("fork failed");
    exit(1);

  } else if (child_pid == 0) {
    printf("Program 2 started.\n");
    sleep(1);

    printf("Program 2 sends signal to program 1.\n");

    kill(getppid(), SIGINT);

    printf("Program 2 finished.\n");
    exit(0);

  } else {
    signal(SIGINT, SIG_DFL);

    while (1) {
      sleep(1);
    }

    printf("Program 1 finished.\n");
  }
}
