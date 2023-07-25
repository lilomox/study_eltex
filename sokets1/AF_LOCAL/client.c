#include <stdio.h>
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
  printf("Waiting message...\n");

  char buf[SIZE];

  recv(server_fd, buf, sizeof(buf), 0);
  printf("Recieved message: %s", buf);

  memset(buf, '\0', SIZE);
  printf("Enter message:");
  fgets(buf, sizeof(buf), stdin);

  send(server_fd, buf, sizeof(buf), 0);
  printf("Message sent\n");

  close(server_fd);
}
