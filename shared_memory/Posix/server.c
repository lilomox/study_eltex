#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define NAME "/shared_mem"
#define SIZE 100

typedef struct {
  char string[SIZE];
} message;

int main() {
  pid_t pid;
  message *msg;
  int shmid;

  shmid = shm_open(NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(shmid, SIZE);
  msg = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

  strcpy(msg->string, "Today is 19th of July");

  pid = fork();

  if (pid == 0) {
    execl("client", "client", NULL);
  } else {
    wait(NULL);
  }
  printf("%s\n", msg->string);

  munmap(msg, SIZE);
  shm_unlink(NAME);
}
