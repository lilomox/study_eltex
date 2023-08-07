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

#define ADDR "224.0.0.1"
#define PORT 7777
#define SIZE 199

int main() {
	struct sockaddr_in server;
	socklen_t len = sizeof(server);

	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1) {
		perror("socket");
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	inet_pton(AF_INET, ADDR, &(server.sin_addr));

	char buf[SIZE];
	strcpy(buf, "Hello!!");

	if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&server, len) == -1) {
		perror("sendto");
		return 1;
	}
}