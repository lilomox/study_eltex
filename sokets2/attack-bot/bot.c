#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define COUNT_OF_BOTS 1000
#define SIZE 255

char buf[SIZE];

void *process(void *arg) {
  pid_t pid;

  pid = fork();
  if (pid == 0) {
    execl(buf, buf, NULL);
  } else {
    wait(NULL);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("You started programm Wrong!\n");
    printf("Example: ./bot 'path to client'\n");
    return 0;
  }
  strcpy(buf, argv[1]);

  pthread_t threads[COUNT_OF_BOTS];

  for (int i = 0; i < COUNT_OF_BOTS; i++) {
    pthread_create(&threads[i], NULL, process, NULL);
  }

  for (int i = 0; i < COUNT_OF_BOTS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
