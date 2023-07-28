#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define PATH "127.0.0.9"
#define PORT_TO_UDP 5555
#define SIZE 199

int main() {
  char buf[SIZE];

  struct sockaddr_in server;
  int fd;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_TO_UDP);
  inet_pton(AF_INET, PATH, &(server.sin_addr));

  char c = 's';
  socklen_t len_cln = sizeof(server);
  sendto(fd, &c, 1, 0, (struct sockaddr *)&server, sizeof(server));

  if (recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&server, &len_cln) ==
      -1) {
    perror("recv");
    return 1;
  }
  printf("%s", buf);

  memset(buf, '\0', SIZE);

  time_t ttime = time(NULL);
  strcpy(buf, ctime(&ttime));

  if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&server, len_cln) ==
      -1) {
    perror("send");
    return 1;
  }
  printf("message sent.\n");

  close(fd);
}
