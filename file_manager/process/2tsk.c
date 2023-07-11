#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t child1, child2, child3, child4, child5;
  int status1, status2, status3, status4, status5;

  child1 = fork();
  if (child1 == 0) {
    printf("pid=%d ppid=%d\n", getpid(), getppid());

    child2 = fork();
    if (child2 == 0) {
      printf("pid=%d ppid=%d\n", getpid(), getppid());

      exit(5);
    } else {
      wait(&status2);

      child3 = fork();
      if (child3 == 0) {
        printf("pid=%d ppid=%d\n", getpid(), getppid());

        exit(5);
      } else {
        wait(&status3);
      }
    }
    exit(5);
  } else {
    printf("source Pid=%d\n", getpid());
    wait(&status1);

    child4 = fork();
    if (child4 == 0) {
      printf("pid=%d ppid=%d\n", getpid(), getppid());
      ;

      child5 = fork();
      if (child5 == 0) {
        printf("pid=%d ppid=%d\n", getpid(), getppid());

        exit(5);
      } else {
        wait(&status5);
      }

      exit(5);
    } else {
      wait(&status4);
    }
  }
}
