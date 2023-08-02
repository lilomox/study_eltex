#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ADDR "127.0.0.2"
#define PORT 8888
#define SIZE 199

int glob_fd = 0;

void handle_sigint(int sig) {
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
}

void handle_sigtstp(int sig) {
  close(glob_fd);
  printf("\n");
  exit(EXIT_SUCCESS);
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

  char buf[SIZE];

  struct sockaddr_in server, client;
  int fd;

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  glob_fd = fd;

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  inet_pton(AF_INET, ADDR, &(server.sin_addr));

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind");
    return 1;
  }
  printf("Waiting conection...\n");

  char c;
  socklen_t len_cln = sizeof(client);
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
