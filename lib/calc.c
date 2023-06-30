#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20

int check_args(int n) {
  if (n < 2) {
    printf("You starting program wrong\n");
    printf("Example: ./calc libsum.so libminus.so ..\n");
    printf("Functions in dynamic-libs should be like EXAMPLE: \033[92mint "
           "\033[93msum\033[39m(\033[92mint\033[39m, \033[92mint\033[39m)\n");
    return 1;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  if (check_args(argc) == 1) {
    return 0;
  }

  char words[argc][NAME_SIZE];
  for (int i = 1; i < argc; i++) {
    int len = strlen(argv[i]);
    for (int j = 3; j < len - 3; j++) {
      words[i][j - 3] = argv[i][j];
    }
  }

  char num[NAME_SIZE];
  while (1) {
    printf("comands: ");
    for (int i = 1; i < argc; i++) {
      printf("%d)%s; ", i, words[i]);
    }
    printf("%d)exit;", argc);
    printf("\nEnter num of command:");
    scanf("%s", num);

    system("clear");

    if (atoi(num) == argc) {
      break;

    } else if ((atoi(num) > 0) && (atoi(num) < argc)) {
      void *handle;
      int (*test)(int, int);

      handle = dlopen(argv[atoi(num)], RTLD_LAZY);
      if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
      }
      dlerror();

      test = (int (*)(int, int))dlsym(handle, words[atoi(num)]);
      if (!test) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
      }
      dlerror();

      int x1, x2;
      printf("Enter two numbers: ");
      scanf("%d %d", &x1, &x2);

      printf("%s(%d, %d) = %d\n", words[atoi(num)], x1, x2, test(x1, x2));
      printf("\n");

      dlclose(handle);

    } else {
      printf("unknown comand\n");
    }
  }
  exit(EXIT_SUCCESS);
}
