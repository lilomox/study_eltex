#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 255

int main() {
  int fd1, fd2;
  pid_t child;

  mkfifo("channel1", 0666);
  mkfifo("channel2", 0666);

  child = fork();
  if (child < 0) {
    perror("fork failed");
    return 0;
  } else if (child == 0) {
    execl("2tsk", "2tsk", NULL);
  } else {
    char msg[SIZE];

    fd1 = open("channel1", O_RDONLY);
    fd2 = open("channel2", O_RDONLY);

    strcpy(msg, "SRC");
    printf("%s\n", msg);
    write(fd2, msg, sizeof(msg));

    wait(NULL);

    read(fd1, msg, sizeof(msg));
    printf("%s\n", msg);

    close(fd1);
    close(fd2);
  }

  unlink("channel1");
  unlink("channel2");
}
