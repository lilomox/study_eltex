#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDR "192.168.122.1"
#define PORT 8889
#define SIZE 199

int main() {
  char buf[SIZE];
  struct sockaddr_in server, client;
  socklen_t len_cln = sizeof(client);
  int fd;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  inet_pton(AF_INET, ADDR, &(server.sin_addr));

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind");
    return 1;
  }
  printf("Waiting conection...\n");

  char c;
  recvfrom(fd, &c, 1, 0, (struct sockaddr *)&client, &len_cln);

  printf("Sent message: Hello!\n");
  strcpy(buf, "Hello!");
  sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&client, len_cln);
  printf("Message sent.\n");

  memset(buf, '\0', SIZE);

  printf("Waiting message...\n");
  recvfrom(fd, buf, SIZE, 0, (struct sockaddr *)&client, &len_cln);
  printf("Recieved message: %s\n", buf);

  close(fd);
}
