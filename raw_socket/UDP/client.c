#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH "127.0.0.2"
#define PORT 7777
#define MAX_SIZE 1000
#define SIZE 199
#define IP_HEAD_SIZE 20

struct msg {
  char buf[SIZE];
};

int main() {
  char buf[MAX_SIZE];

  struct sockaddr_in server;

  socklen_t len = sizeof(server);

  struct udphdr *udp_header;
  struct msg *message;

  udp_header = (struct udphdr *)buf;
  message = (struct msg *)(buf + sizeof(struct udphdr));

  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  server.sin_family = AF_INET;
  server.sin_port = htons(8888);
  inet_pton(AF_INET, PATH, &(server.sin_addr));

  udp_header->source = htons(PORT);
  udp_header->dest = htons(8888);
  udp_header->len = htons(sizeof(struct udphdr) + 1);
  udp_header->check = 0;

  strcpy(message->buf, "s");

  if (sendto(fd, buf, sizeof(struct udphdr) + 1, 0, (struct sockaddr *)&server,
             len) < 0) {
    perror("sendto");
    return 1;
  }

  memset(message->buf, '\0', SIZE);
  printf("Waiting message...\n");

  while (1) {
    recvfrom(fd, buf, MAX_SIZE, 0, (struct sockaddr *)&server, &len);
    udp_header = (struct udphdr *)(buf + IP_HEAD_SIZE);
    message = (struct msg *)(buf + sizeof(struct udphdr) + (sizeof(int) * 5));
    if (ntohs(udp_header->dest) == 7777) {
      break;
    }
  }

  printf("Recieved message: %s\n", message->buf);
  sleep(1);

  char temp[SIZE];
  strcpy(temp, message->buf);

  char sec_buf[MAX_SIZE];
  udp_header = (struct udphdr *)sec_buf;
  message = (struct msg *)(sec_buf + sizeof(struct udphdr));

  strcpy(message->buf, temp);
  strcat(message->buf, " =)");
  printf("Sent message: %s\n", message->buf);

  udp_header->source = htons(PORT);
  udp_header->dest = htons(8888);
  udp_header->len = htons(sizeof(struct udphdr) + strlen(message->buf));
  udp_header->check = 0;

  if (sendto(fd, sec_buf, sizeof(struct udphdr) + strlen(message->buf), 0,
             (struct sockaddr *)&server, len) < 0) {
    perror("sendto");
    return 1;
  }
  printf("Message sent.\n");

  close(fd);
}
