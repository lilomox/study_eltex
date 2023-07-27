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

int main() {
  struct sockaddr_un server;
  int server_fd;

  server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  server.sun_family = AF_LOCAL;
  strcpy(server.sun_path, PATH);

  if (connect(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return 1;
  }

  char buf[SIZE];
  if (recv(server_fd, buf, sizeof(buf), 0) == -1) {
    perror("recv");
    return 1;
  }
  printf("%s", buf);

  memset(buf, '\0', SIZE);

  printf("Enter message: ");
  fgets(buf, SIZE, stdin);

  if (send(server_fd, buf, strlen(buf), 0) == -1) {
    perror("send");
    return 1;
  }
  printf("message sent.\n");
}
