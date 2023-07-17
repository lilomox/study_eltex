#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 255

int main() {
	char arr[SIZE];
	int count = 0;
	int readfd;

	readfd = open("buf", O_RDONLY, 0);

	char c;
	while (read(readfd, &c, 1) > 0) {
		arr[count] = c;
		count++;
	}

	printf("hey, %s\n", arr);

	close(readfd);
}