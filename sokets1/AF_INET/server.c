#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDR "127.0.0.2"
#define PORT 7777
#define SIZE 199

int main() {
	char buf[SIZE];

	struct sockaddr_in server, client;
	int fd;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

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

	printf("Enter message: ");
	fgets(buf, SIZE, stdin);
	sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&client, len_cln);
	printf("Message sent.\n");

	memset(buf, '\0', SIZE);

	printf("Waiting message...\n");
	recvfrom(fd, buf, SIZE, 0, (struct sockaddr *)&client, &len_cln);
	printf("Recieved message: %s", buf);

	close(fd);
}