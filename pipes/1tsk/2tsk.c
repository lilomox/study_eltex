#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pipefd[2];
  pid_t pid;

  pipe(pipefd);
  pid = fork();

  if (pid == 0) {
    close(pipefd[0]);
    printf("Hello\n");
    write(pipefd[1], "Hello", strlen("Hello"));
    close(pipefd[1]);
    exit(EXIT_SUCCESS);

  } else {
    close(pipefd[1]);
    wait(NULL);
    char c;
    while (read(pipefd[0], &c, 1) > 0) {
      write(STDOUT_FILENO, &c, 1);
    }
    write(STDOUT_FILENO, ", bye\n", 6);
    close(pipefd[0]);
    exit(EXIT_SUCCESS);
  }
}
