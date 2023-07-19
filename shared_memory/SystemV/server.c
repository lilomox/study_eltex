#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define N 100

typedef struct {
	char string[N];
}message;

int main() {
	pid_t pid;
	message *msg;
	key_t key;
	int shmid;

	key = ftok("server.c", 1);
	shmid = shmget(key, N,  IPC_CREAT | 0666);
	msg = shmat(shmid, NULL, 0);
	strcpy(msg->string, "Hello");

	pid = fork();

	if (pid == 0) {
		execl("client", "client", NULL);
	} else {
		wait(NULL);
	}
	printf("%s\n", msg->string);

	shmdt((void *)msg);
	shmctl(shmid, IPC_RMID, NULL);
}