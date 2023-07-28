#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

#define PATH "127.0.0.14"
#define SIZE 199
#define PORT_TO_TCP 6666

int main() {
  struct sockaddr_in server;
  int server_fd;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_TO_TCP);
  inet_pton(AF_INET, PATH, &(server.sin_addr));

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

  time_t ttime = time(NULL);
  strcpy(buf, ctime(&ttime));

  if (send(server_fd, buf, strlen(buf), 0) == -1) {
    perror("send");
    return 1;
  }
  printf("message sent.\n");
}
