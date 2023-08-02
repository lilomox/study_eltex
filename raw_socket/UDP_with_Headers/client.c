#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH "127.0.0.2"
#define PORT 7777
#define DEST_PORT 8888
#define MAX_SIZE 1000
#define SIZE 199
#define IP_HEAD_SIZE 20

struct msg {
  char buf[SIZE];
};

int main() {
  char buf[MAX_SIZE];
  int fd;
  struct sockaddr_in server;
  socklen_t len = sizeof(server);

  struct iphdr *ip_header;
  struct udphdr *udp_header;
  struct msg *message;

  ip_header = (struct iphdr *)buf;
  udp_header = (struct udphdr *)(buf + sizeof(struct iphdr));
  message = (struct msg *)(buf + sizeof(struct iphdr) + sizeof(struct udphdr));

  fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (fd == -1) {
    perror("socket");
    return 1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(DEST_PORT);
  inet_pton(AF_INET, PATH, &(server.sin_addr));

  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tos = 0;
  ip_header->tot_len = 0;
  ip_header->id = 0;
  ip_header->frag_off = 0;
  ip_header->ttl = 255;
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = 0;
  ip_header->saddr = inet_addr(PATH);
  ip_header->daddr = inet_addr(PATH);

  int flag = 1;
  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) == -1) {
    perror("setsockopt");
    return 1;
  }

  udp_header->source = htons(PORT);
  udp_header->dest = htons(DEST_PORT);
  udp_header->len = htons(sizeof(struct udphdr) + 1);
  udp_header->check = 0;

  strcpy(message->buf, "s");

  if (sendto(fd, buf, sizeof(struct iphdr) + sizeof(struct udphdr) + 1, 0,
             (struct sockaddr *)&server, len) < 0) {
    perror("sendto");
    return 1;
  }

  memset(message->buf, '\0', SIZE);
  printf("Waiting message...\n");

  while (1) {
    recvfrom(fd, buf, MAX_SIZE, 0, (struct sockaddr *)&server, &len);
    ip_header = (struct iphdr *)buf;
    udp_header = (struct udphdr *)(buf + sizeof(struct iphdr));
    message =
        (struct msg *)(buf + sizeof(struct iphdr) + sizeof(struct udphdr));
    if (ntohs(udp_header->dest) == PORT) {
      break;
    }
  }

  printf("Recieved message: %s\n", message->buf);
  sleep(1);

  char temp[SIZE];
  strcpy(temp, message->buf);

  char sec_buf[MAX_SIZE];
  ip_header = (struct iphdr *)sec_buf;
  udp_header = (struct udphdr *)(sec_buf + sizeof(struct iphdr));
  message =
      (struct msg *)(sec_buf + sizeof(struct iphdr) + sizeof(struct udphdr));

  strcpy(message->buf, temp);
  strcat(message->buf, " =)");
  printf("Sent message: %s\n", message->buf);

  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tos = 0;
  ip_header->tot_len = 0;
  ip_header->id = 0;
  ip_header->frag_off = 0;
  ip_header->ttl = 255;
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = 0;
  ip_header->saddr = inet_addr(PATH);
  ip_header->daddr = inet_addr(PATH);

  udp_header->source = htons(PORT);
  udp_header->dest = htons(DEST_PORT);
  udp_header->len = htons(sizeof(struct udphdr) + strlen(message->buf));
  udp_header->check = 0;

  if (sendto(fd, sec_buf,
             sizeof(struct iphdr) + sizeof(struct udphdr) +
                 strlen(message->buf),
             0, (struct sockaddr *)&server, len) < 0) {
    perror("sendto");
    return 1;
  }
  printf("Message sent.\n");

  close(fd);
}
