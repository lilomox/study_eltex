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

  struct sockaddr_in server, client;
  socklen_t addrlen;
  int server_fd, client_fd;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  inet_pton(AF_INET, ADDR, &(server.sin_addr));

  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind");
    return 1;
  }

  if (listen(server_fd, 5) == -1) {
    perror("listen");
    return 1;
  }
  printf("Waiting conection...\n");

  addrlen = sizeof(client);

  printf("Enter message: ");
  client_fd = accept(server_fd, (struct sockaddr *)&client, &addrlen);
  if (client_fd == -1) {
    perror("accept");
    return 1;
  }

  fgets(buf, SIZE, stdin);
  send(client_fd, buf, strlen(buf), 0);
  printf("Message sent.\n");
  memset(buf, '\0', SIZE);

  printf("Waiting message...\n");
  recv(client_fd, buf, sizeof(buf), 0);
  printf("Recieved message: %s", buf);

  close(server_fd);
  close(client_fd);
}
