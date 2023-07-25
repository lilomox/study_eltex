#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define SIZE 255

typedef struct {
  long mtype;
  char mtext[SIZE];
} msgbuf;

int main() {
  key_t key;
  msgbuf rcv, snd;

  key = ftok("server", 1);
  int msqid = msgget(key, IPC_CREAT | 0666);

  msgrcv(msqid, &rcv, sizeof(msgbuf), 0, IPC_NOWAIT);
  printf("sent message: %s\n", rcv.mtext);

  snd.mtype = 3;
  strcpy(snd.mtext, "Hi, I'm Maksi %)");

  msgsnd(msqid, &snd, sizeof(msgbuf) - sizeof(long), 0);
}
