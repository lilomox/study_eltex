#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define PATH "/tmp/sockets"
#define SIZE 199
#define COUNT_OF_THREADS 5

int glob_fd = 0;

struct msgbuf {
  long mtype;
  int mtext;
};

pthread_mutex_t mutexes[COUNT_OF_THREADS];

void handle_sigint(int sig) {
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_mutex_destroy(&mutexes[i]);
  }
  unlink(PATH);
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void handle_sigtstp(int sig) {
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_mutex_destroy(&mutexes[i]);
  }
  unlink(PATH);
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void *process(void *arg) {
  int *tmp = (int *)arg;
  int i = *tmp;

  int msqid;
  key_t key;
  struct msgbuf buff;

  key = ftok(".", 'm');
  msqid = msgget(key, IPC_CREAT | 0666);
  if (msqid == -1) {
    perror("msgget");
    exit(1);
  }
  if (pthread_mutex_trylock(&mutexes[i]) == 0) {
    if (msgrcv(msqid, &buff, sizeof(buff.mtext), 0, 0) == -1) {
      perror("msgrcv");
      exit(1);
    }

    int temp_fd = buff.mtext;
    if (send(temp_fd, "connection successful\n",
             strlen("connection successful\n"), 0) == -1) {
      perror("send");
      exit(1);
    }

    char buf[SIZE];
    if (recv(temp_fd, buf, sizeof(buf), 0) == -1) {
      perror("recv");
      exit(1);
    }

    printf("receive massage: %s", buf);

    pthread_mutex_unlock(&mutexes[i]);
  }

  return NULL;
}

int main() {
  int msqid;
  key_t key;
  struct msgbuf buf;

  key = ftok(".", 'm');
  msqid = msgget(key, IPC_CREAT | 0666);
  if (msqid == -1) {
    perror("msgget");
    return 1;
  }

  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_mutex_init(&mutexes[i], NULL);
  }

  if (signal(SIGINT, handle_sigint) == SIG_ERR) {
    perror("signal SIGINT");
    return 1;
  }

  if (signal(SIGTSTP, handle_sigtstp) == SIG_ERR) {
    perror("signal SIGTSTP");
    return 1;
  }

  struct sockaddr_un server, client;
  int server_fd, client_fd;
  socklen_t addrlen = sizeof(client);

  server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket in Server");
    return 1;
  }
  glob_fd = server_fd;

  server.sun_family = AF_LOCAL;
  strcpy(server.sun_path, PATH);

  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind");
    return 1;
  }

  if (listen(server_fd, 5) == -1) {
    perror("listen");
    return 1;
  }

  pthread_t treads[COUNT_OF_THREADS];
  int arr[COUNT_OF_THREADS];
  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    arr[i] = i;
    pthread_create(&treads[i], NULL, process, &arr[i]);
  }

  while (1) {
    client_fd = accept(server_fd, (struct sockaddr *)&server, &addrlen);
    if (client_fd == -1) {
      perror("accept");
      return 1;
    }
    buf.mtype = 1;
    buf.mtext = client_fd;
    if (msgsnd(msqid, &buf, sizeof(buf.mtext), 0) == -1) {
      perror("msgsnd 1");
      return 1;
    }
  }

  for (int i = 0; i < COUNT_OF_THREADS; i++) {
    pthread_join(treads[i], NULL);
  }
}
