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
  char word[SIZE];
  int fd1, fd2;

  fd1 = open("channel1", O_WRONLY);
  fd2 = open("channel2", O_RDONLY);

  read(fd2, word, sizeof(word));
  printf("%s\n", word);

  strcpy(word, "sendd");
  write(fd1, word, sizeof(word));

  close(fd1);
  close(fd2);
  return 0;
}
