#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define PATH "/tmp/sockets"
#define SIZE 199

int main() {
  struct sockaddr_un server, client;
  int server_fd, client_fd, bind_res;
  socklen_t addrlen;

  server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket");
    return 1;
  }

  server.sun_family = AF_LOCAL;
  strcpy(server.sun_path, PATH);
  unlink(PATH);
  bind_res = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
  if (bind_res == -1) {
    perror("bind");
    return 1;
  }

  if (listen(server_fd, 5) == -1) {
    perror("listen");
    return 1;
  }

  addrlen = sizeof(client);

  printf("Waiting conection...\n");
  client_fd = accept(server_fd, (struct sockaddr *)&client, &addrlen);
  if (client_fd == -1) {
    perror("accept");
    return 1;
  }

  char buf[SIZE];

  printf("Enter message: ");
  fgets(buf, sizeof(buf), stdin);

  send(client_fd, buf, strlen(buf), 0);
  printf("Message sent\n");

  memset(buf, '\0', SIZE);
  printf("Waiting message...\n");
  recv(client_fd, buf, sizeof(buf), 0);
  printf("Recieved message: %s", buf);

  if (unlink(PATH) < 0) {
    perror("unlink");
    return 1;
  }

  close(server_fd);
}
