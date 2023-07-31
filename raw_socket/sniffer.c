#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX_BUFFER_SIZE 65536

int main() {
  int count = 0;
  char buf[MAX_BUFFER_SIZE];

  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  while (1) {
    if (recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL) < 0) {
      perror("recvfrom");
      return 1;
    }
    count++;
    printf("Received udp packets: %d\n", count);
  }

  return 0;
}
