#include <stdio.h>

int main() {
  int x = 270533154;
  char *pointer = &x;

  printf("x = %d\n", x);
  printf("Bits of source num x: ");
  for (int i = 0; i < 4; i++) {
    printf("%d ", pointer[i]);
  }
  printf("\n");

  pointer[2] = 127;

  printf("Changed x = %d\n", x);
  printf("Bits of changed num x: ");
  for (int i = 0; i < 4; i++) {
    printf("%d ", pointer[i]);
  }
  printf("\n");
}
