#include <curses.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define SIZE 255

int stok(char *src, char symb, char **strs) {
  int i = 0;
  int numstrs = 1;
  strs[i] = src;
  while (src[i] != '\0') {
    if (src[i] == symb) {
      src[i] = '\0';
      strs[numstrs] = &(src[i]) + 1;
      numstrs++;
    }
    i++;
  }
  return numstrs;
}

int find_last_slesh(char *str) {
  int len = strlen(str);
  int poz = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == '/') {
      poz = i;
    }
  }
  return poz;
}

int main() {
  pid_t pid;

  char *path = malloc(SIZE);
  getcwd(path, SIZE);
  int poz_of_slesh = find_last_slesh(path);
  strcat(path, "/");
  while (1) {
    char *arr = malloc(SIZE);
    char *comand = malloc(SIZE);
    int co_of_cmd = 0;

    printf("%s:", path);
    char tmp;
    while ((tmp = getchar()) != '\n') {
      comand[co_of_cmd] = tmp;
      co_of_cmd++;
    }
    comand[co_of_cmd] = '\0';

    if (strcmp(comand, "quit") == 0) {
      break;
    } else {
      char *words[SIZE];
      if (stok(comand, ' ', words) == 1) {
        pid = fork();
        if (pid == 0) {
          char *tmp_word = malloc(SIZE);
          strcpy(tmp_word, path);
          strcat(tmp_word, comand);
          if (execl(tmp_word, tmp_word, NULL) == -1) {
            char *tmp_arr = malloc(SIZE);
            strcat(tmp_arr, "/usr/bin/");
            strcat(tmp_arr, comand);
            if (execl(tmp_arr, comand, NULL) == -1) {
              printf("Unknown comand\n");
            }
            free(tmp_arr);
            exit(EXIT_SUCCESS);
          }
          free(tmp_word);
        } else {
          wait(NULL);
        }
      } else {
        if (strcmp(words[0], "cd") == 0) {
          char *tmp_path = malloc(SIZE);
          strcpy(tmp_path, path);
          if (strcmp(words[1], "..") == 0) {
            path[poz_of_slesh] = '\0';
            chdir(path);
            poz_of_slesh = find_last_slesh(path);
            strcat(path, "/");
          } else {
            strcat(tmp_path, words[1]);
            if (opendir(tmp_path) != NULL) {
              chdir(tmp_path);
              strcpy(path, tmp_path);
              poz_of_slesh = find_last_slesh(path);
              strcat(path, "/");
            } else {
              printf("Unknown dir\n");
            }
          }
          free(tmp_path);
        } else if (strcmp(words[0], ">") == 0) {
          char *tmp_file = malloc(SIZE);
          strcpy(tmp_file, path);
          strcat(tmp_file, words[1]);
          int file = open(tmp_file, O_CREAT, S_IRWXU);
          close(file);
          free(tmp_file);
        } else {
          pid = fork();
          if (pid == 0) {
            char *tmp_word = malloc(SIZE);
            strcpy(tmp_word, path);
            strcat(tmp_word, words[1]);
            if (execl(words[0], words[0], words[1], NULL) == -1) {
              char *tmp_arr = malloc(SIZE);
              strcat(tmp_arr, "/usr/bin/");
              strcat(tmp_arr, words[0]);
              if (execl(tmp_arr, words[0], tmp_word, NULL) == -1) {
                printf("Unknown comand\n");
              }
              free(tmp_arr);
              exit(EXIT_SUCCESS);
            }
            free(tmp_word);
          } else {
            wait(NULL);
          }
        }
      }
    }

    free(arr);
    free(comand);
  }
  free(path);
}
