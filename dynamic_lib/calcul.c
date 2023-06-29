#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions/devid.h"
#include "functions/minus.h"
#include "functions/multiply.h"
#include "functions/sum.h"

#define N 10

int main() {
  char num[N];

  while (1) {
    printf(
        "Command of calculator: 1)plus 2)minus 3)multiply 4)divide 5)exit\n");
    printf("Enter num of command: ");
    scanf("%s", num);

    int x, y;
    system("clear");
    if (strcmp(num, "1") == 0) {
      printf("Enter 2 'int' numbers to plusing: ");
      scanf("%d %d", &x, &y);
      printf("%d + %d = %d\n", x, y, sum(x, y));
      printf("\n");

    } else if (strcmp(num, "2") == 0) {
      printf("Enter 2 'int' numbers to minusing: ");
      scanf("%d %d", &x, &y);
      printf("%d - %d = %d\n", x, y, minus(x, y));
      printf("\n");

    } else if (strcmp(num, "3") == 0) {
      printf("Enter 2 'int' numbers to multiply: ");
      scanf("%d %d", &x, &y);
      printf("%d * %d = %d\n", x, y, multiply(x, y));
      printf("\n");

    } else if (strcmp(num, "4") == 0) {
      printf("Enter 2 'int' numbers to deviding: ");
      scanf("%d %d", &x, &y);
      printf("%d / %d = %d\n", x, y, devid(x, y));
      printf("\n");

    } else if (strcmp(num, "5") == 0) {
      break;
    } else {
      printf("Unknown command\n");
      printf("\n");
    }
  }
}
