#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 255

int main() {
  pid_t pid;
  int writefd;
  pid = fork();

  if (pid == 0) { 
    execl("2tsk", "2tsk", NULL);
  } else { 
    writefd = open("buf", O_WRONLY, 0);

    char arr[SIZE];
    printf("Enter your name: ");
    scanf("%s", arr);

    write(writefd, arr, strlen(arr));

    close(writefd);
    wait(NULL);
    exit(EXIT_SUCCESS);
  }
}