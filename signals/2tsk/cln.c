#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Starting programm Wrong\n");
    printf("After name of programm you should wrote a PID\n");
    return 0;
  }
  printf("Program 2 started.\n");

  pid_t p1_pid = atoi(argv[1]);

  kill(p1_pid, SIGINT);

  printf("Program 2 finished.\n");
}
