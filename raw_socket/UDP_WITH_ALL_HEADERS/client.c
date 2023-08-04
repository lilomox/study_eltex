#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define PATH "192.168.88.242"
#define DEST_PATH "192.168.8.0"
#define PORT 7777
#define DEST_PORT 8888
#define MAX_SIZE 10000
#define SIZE 199

unsigned char src_mac[6] = {0x38, 0x87, 0xd5, 0xc8, 0x12, 0x16};
unsigned char dest_mac[6] = {0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11}; // input dest mac

struct msg {
  char buf[SIZE];
};

unsigned short check_sum(unsigned short *ptr, int nbytes) {
  register long sum = 0;
  unsigned short oddbyte;
  register short answer;

  while (nbytes > 1) {
    sum += *ptr++;
    nbytes -= 2;
  }
  if (nbytes == 1) {
    oddbyte = 0;
    *((u_char *)&oddbyte) = *(u_char *)ptr;
    sum += oddbyte;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum = sum + (sum >> 16);
  answer = (short)~sum;

  return (answer);
}

int main() {
  char buf[MAX_SIZE];
  int fd;
  struct sockaddr_ll server;
  socklen_t len = sizeof(server);

  memset(&server, 0, sizeof(struct sockaddr_ll));
  server.sll_family = AF_PACKET;
  server.sll_ifindex = if_nametoindex("wlo1");
  server.sll_halen = 6;
  for (int i = 0; i < 6; i++) {
    server.sll_addr[i] = src_mac[i];
  }

  struct ether_header *mac;
  struct iphdr *ip_header;
  struct udphdr *udp_header;
  struct msg *message;

  mac = (struct ether_header *)buf;
  ip_header = (struct iphdr *)(buf + sizeof(struct ether_header));
  udp_header = (struct udphdr *)(buf + sizeof(struct ether_header) +
                                 sizeof(struct iphdr));
  message = (struct msg *)(buf + sizeof(struct ether_header) +
                           sizeof(struct iphdr) + sizeof(struct udphdr));

  fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (fd == -1) {
    perror("socket");
    return 1;
  }

  for (int i = 0; i < 6; i++) {
    mac->ether_shost[i] = src_mac[i];
    mac->ether_dhost[i] = dest_mac[i];
  }
  mac->ether_type = htons(0x0800);

  strcpy(message->buf, "s");

  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tos = 0;
  ip_header->tot_len =
      htons(sizeof(struct iphdr) + sizeof(udp_header) + strlen(message->buf));
  ip_header->id = htonl(12345);
  ip_header->frag_off = 0;
  ip_header->ttl = 255;
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = 0;
  ip_header->saddr = inet_addr(PATH);
  ip_header->daddr = inet_addr(DEST_PATH);
  ip_header->check =
      check_sum((unsigned short *)ip_header, sizeof(struct iphdr));

  udp_header->source = htons(PORT);
  udp_header->dest = htons(DEST_PORT);
  udp_header->len = htons(sizeof(struct udphdr) + strlen(message->buf));
  udp_header->check = 0;

  if (sendto(fd, buf,
             sizeof(struct ether_header) + sizeof(struct iphdr) +
                 sizeof(struct udphdr) + 1,
             0, (struct sockaddr *)&server, len) < 0) {
    perror("sendto");
    return 1;
  }

  memset(message->buf, '\0', SIZE);
  memset(&server, 0, sizeof(struct sockaddr_ll));
  printf("Waiting message...\n");

  while (1) {
    recvfrom(fd, buf, MAX_SIZE, 0, (struct sockaddr *)&server, &len);
    mac = (struct ether_header *)buf;
    ip_header = (struct iphdr *)(buf + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(buf + sizeof(struct ether_header) +
                                   sizeof(struct iphdr));
    message = (struct msg *)(buf + sizeof(struct ether_header) +
                             sizeof(struct iphdr) + sizeof(struct udphdr));
    if (ntohs(udp_header->dest) == PORT) {
      break;
    }
  }

  printf("Recieved message: %s\n", message->buf);
  sleep(1);

  char temp[SIZE];
  strcpy(temp, message->buf);

  char sec_buf[MAX_SIZE];
  mac = (struct ether_header *)sec_buf;
  ip_header = (struct iphdr *)(sec_buf + sizeof(struct ether_header));
  udp_header = (struct udphdr *)(sec_buf + sizeof(struct ether_header) +
                                 sizeof(struct iphdr));
  message = (struct msg *)(sec_buf + sizeof(struct ether_header) +
                           sizeof(struct iphdr) + sizeof(struct udphdr));

  strcpy(message->buf, temp);
  strcat(message->buf, " =)");
  printf("Sent message: %s\n", message->buf);

  memset(&server, 0, sizeof(struct sockaddr_ll));
  server.sll_family = AF_PACKET;
  server.sll_ifindex = if_nametoindex("wlo1");
  server.sll_halen = 6;
  for (int i = 0; i < 6; i++) {
    server.sll_addr[i] = src_mac[i];
  }

  for (int i = 0; i < 6; i++) {
    mac->ether_shost[i] = src_mac[i];
    mac->ether_dhost[i] = dest_mac[i];
  }
  mac->ether_type = htons(0x0800);

  ip_header->version = 4;
  ip_header->ihl = 5;
  ip_header->tos = 0;
  ip_header->tot_len =
      htons(sizeof(struct iphdr) + sizeof(udp_header) + strlen(message->buf));
  ip_header->id = htonl(12345);
  ip_header->frag_off = 0;
  ip_header->ttl = 255;
  ip_header->protocol = IPPROTO_UDP;
  ip_header->check = 0;
  ip_header->saddr = inet_addr(PATH);
  ip_header->daddr = inet_addr(DEST_PATH);
  ip_header->check =
      check_sum((unsigned short *)ip_header, sizeof(struct iphdr));

  udp_header->source = htons(PORT);
  udp_header->dest = htons(DEST_PORT);
  udp_header->len = htons(sizeof(struct udphdr) + strlen(message->buf));
  udp_header->check = 0;

  if (sendto(fd, sec_buf,
             sizeof(struct ether_header) + sizeof(struct iphdr) +
                 sizeof(struct udphdr) + strlen(message->buf),
             0, (struct sockaddr *)&server, len) < 0) {
    perror("sendto");
    return 1;
  }
  printf("Message sent.\n");

  close(fd);
}
