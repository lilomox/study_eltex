#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 255

typedef struct {
	long mtype;
	char mtext[SIZE];
}msgbuf;

int main() {
	struct msqid_ds info;

	pid_t pid;

	key_t key; 
	msgbuf buf, rcv;

	key = ftok("server", 1);
	int msqid = msgget(key, IPC_CREAT | 0666);

	buf.mtype = 1;
	strcpy(buf.mtext, "Hello, I'm Lilo ^-^");

	msgsnd(msqid, &buf, sizeof(msgbuf) - sizeof(long), 0);

	pid = fork();

	if (pid == 0) {
		execl("client", "client", NULL);
	} else {
		wait(NULL);
	}

	msgrcv(msqid, &rcv, sizeof(msgbuf), 3,  IPC_NOWAIT);
	printf("received message: %s\n", rcv.mtext);

	msgctl(msqid, IPC_RMID, &info);
}