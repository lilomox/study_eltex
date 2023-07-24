#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("My PID = %d\n", getpid());
  signal(SIGINT, SIG_DFL);

  printf("Program 1 started.\n");

  while (1) {
    sleep(1);
  }

  printf("Program 1 finished.\n");
}
