#include <stdio.h>

int main()
{
	int x = 0xDDCCBBAA;
	int counter = 24;
	printf("Source num: ");
	for (; counter >= 0; counter -= 8) {
		printf("%X", (x >> counter) & 0x000000FF);
	}
	printf("\n");

	x &= 0xFFFF00FF;
	x |= 0x0000EE00;
	
	printf("Changed num: ");
	for (counter = 24; counter >= 0; counter -= 8) {
                printf("%X", (x >> counter) & 0x000000FF);
        }
        printf("\n");
}
