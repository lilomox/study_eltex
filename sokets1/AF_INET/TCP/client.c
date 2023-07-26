#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDR "127.0.0.3"
#define PORT 8888
#define SIZE 199

int main() {
  char buf[SIZE];

  struct sockaddr_in server;
  int fd;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  inet_pton(AF_INET, ADDR, &(server.sin_addr));

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return 1;
  }
  printf("Waiting message...\n");

  recv(fd, buf, sizeof(buf), 0);
  printf("Recieved message: %s", buf);
  memset(buf, '\0', SIZE);

  printf("Enter message: ");
  fgets(buf, SIZE, stdin);

  send(fd, buf, strlen(buf), 0);
  printf("Message sent.\n");

  close(fd);
}
