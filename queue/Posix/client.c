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
  char buf[SIZE];

  mqd_t mqd;
  mqd = mq_open(NAME, O_RDWR);
  unsigned int msg_pri;

  mq_receive(mqd, buf, sizeof(buf), &msg_pri);

  printf("sent message: %s\n", buf);

  mq_send(mqd, "I'm OK!", strlen("I'm OK!") + 1, 0);
}
