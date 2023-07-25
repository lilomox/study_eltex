#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define NAME "/queue"
#define SIZE 255

int main() {
  pid_t pid;

  char buf[SIZE] = "Heeeey, how are U??";
  char receive_buf[SIZE];

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = SIZE;
  attr.mq_curmsgs = 0;

  mqd_t mqd;
  mqd = mq_open(NAME, O_CREAT | O_RDWR, 0666, &attr);
  unsigned int msg_pri;

  mq_send(mqd, buf, strlen(buf) + 1, 0);

  pid = fork();

  if (pid == 0) {
    execl("client", "client", NULL);
  } else {
    wait(NULL);
  }

  ssize_t len = mq_receive(mqd, receive_buf, sizeof(receive_buf), &msg_pri);
  if (len == -1) {
    perror("mq_receive");
  }
  printf("recieve message: %s\n", receive_buf);

  mq_unlink(NAME);
  mq_close(mqd);
}
