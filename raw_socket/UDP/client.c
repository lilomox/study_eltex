#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDR "127.0.0.1"
#define PORT 7777
#define SIZE 199

int main() {
  char buf[SIZE];

  struct sockaddr_in server;
  int fd;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  inet_pton(AF_INET, ADDR, &(server.sin_addr));

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return 1;
  }
  char c = 's';
  socklen_t len_cln = sizeof(server);
  sendto(fd, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));

  printf("Waiting message...\n");

  recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&server, &len_cln);
  printf("Recieved message: %s\n", buf);

  printf("Sent message: Hello!, 876\n");
  strcat(buf, ", 876");

  sleep(1);
  sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));
  printf("Message sent.\n");

  close(fd);
}
