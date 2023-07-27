#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define PATH "/tmp/sockets"
#define SIZE 199

int glob_fd = 0;

void handle_sigint(int sig) {
  unlink(PATH);
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void handle_sigtstp(int sig) {
  unlink(PATH);
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void *process(void *arg) {
  int *temp = (int *)arg;
  int temp_fd = *temp;

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

  return NULL;
}

int main() {
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

  pthread_t treads[100];
  int count_of_thr = 0;
  while (1) {
    client_fd = accept(server_fd, (struct sockaddr *)&server, &addrlen);
    if (client_fd == -1) {
      perror("accept");
      return 1;
    }
    printf("%d\n", client_fd);
    pthread_create(&treads[count_of_thr], NULL, process, &client_fd);

    count_of_thr++;
  }

  for (int i = 0; i < count_of_thr; i++) {
    pthread_join(treads[i], NULL);
  }
}
