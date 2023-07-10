#include <curses.h>
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MAX_PATH_LEN 30000
#define NAME_SIZE 70
#define LEN 30000

void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

typedef struct List {
  int poz;
  int local;
  char str[LEN][NAME_SIZE];
  char path[MAX_PATH_LEN];
} List;

int main() {
  system("clear");
  List list[2];
  list[0].poz = 1;
  list[1].poz = 1;
  strcpy(list[0].path, "./");
  strcpy(list[1].path, "./");

  int flag = 1;
  int glob = 0;
  int glob_side = 0;
  int glob_cols = 1;

  while (flag == 1) {
    initscr();
    signal(SIGWINCH, sig_winch);
    idlok(stdscr, TRUE);
    keypad(stdscr, TRUE);
    WINDOW *wnd1, *wnd2;
    WINDOW *subwnd1, *subwnd2;

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    int row, cols;
    row = 1;
    cols = glob_cols;

    refresh();
    noecho();
    wnd1 = newwin(32, COLS / 2, 0, 0);
    box(wnd1, '|', '=');
    subwnd1 = derwin(wnd1, 30, (COLS / 2) - 2, 1, 1);
    wrefresh(wnd1);

    wnd2 = newwin(32, COLS / 2, 0, COLS / 2);
    box(wnd2, '|', '=');
    subwnd2 = derwin(wnd2, 30, (COLS / 2) - 2, 1, 1);
    wrefresh(wnd2);

    struct dirent **namelist1, **namelist2;

    int n1, n2;
    int tl = strlen(list[0].path);
    int tl1 = strlen(list[1].path);

    n1 = scandir(list[0].path, &namelist1, 0, alphasort);
    n2 = scandir(list[1].path, &namelist2, 0, alphasort);

    for (int i = 1; i < ((n1 > 31) ? 31 : n1); i++) {
      strcat(list[0].path, namelist1[i]->d_name);
      DIR *drst = opendir(list[0].path);
      if (drst == NULL) {
        wattron(subwnd1, COLOR_PAIR(1));

        wattron(subwnd1, A_BOLD);

        wprintw(subwnd1, "%s\n", namelist1[i]->d_name);

        wattroff(subwnd1, A_BOLD);

        wattroff(subwnd1, COLOR_PAIR(1));

        wrefresh(subwnd1);
      } else {
        wattron(subwnd1, COLOR_PAIR(2));

        wattron(subwnd1, A_BOLD);

        wprintw(subwnd1, "%s\n", namelist1[i]->d_name);

        wattroff(subwnd1, A_BOLD);

        wattroff(subwnd1, COLOR_PAIR(2));

        wrefresh(subwnd1);
        closedir(drst);
      }
      list[0].path[tl] = '\0';
    }

    for (int i = 1; i < n1; i++) {
      strcpy(list[0].str[i], namelist1[i]->d_name);
      free(namelist1[i]);
    }
    free(namelist1);
    list[0].poz = n1;

    for (int i = 1; i < ((n2 > 31) ? 31 : n2); i++) {
      strcat(list[1].path, namelist2[i]->d_name);
      DIR *drst1 = opendir(list[1].path);
      if (drst1 == NULL) {
        wattron(subwnd2, COLOR_PAIR(1));

        wattron(subwnd2, A_BOLD);

        wprintw(subwnd2, "%s\n", namelist2[i]->d_name);

        wattroff(subwnd2, A_BOLD);

        wattroff(subwnd2, COLOR_PAIR(1));

        wrefresh(subwnd2);
      } else {
        wattron(subwnd2, COLOR_PAIR(2));

        wattron(subwnd2, A_BOLD);

        wprintw(subwnd2, "%s\n", namelist2[i]->d_name);

        wattroff(subwnd2, A_BOLD);

        wattroff(subwnd2, COLOR_PAIR(2));

        wrefresh(subwnd2);
        closedir(drst1);
      }
      list[1].path[tl1] = '\0';
    }

    for (int i = 1; i < n2; i++) {
      strcpy(list[1].str[i], namelist2[i]->d_name);
      free(namelist2[i]);
    }
    free(namelist2);
    list[1].poz = n2;

    list[1].local = list[0].local = 1;
    int side = glob_side;

    wchar_t c;
    flag = 0;

    while (c != '~') {
      move(row, cols);
      c = getch();

      if (c == KEY_BTAB) {
        if (side == 0) {
          cols = (COLS / 2) + 1;
          side = 1;
        } else {
          cols = 1;
          side = 0;
        }
        row = 1;
        list[side].local = 1;

      } else if (c == KEY_UP) {
        if (row > 1) {
          row--;
          list[side].local--;
        } else {
          if (list[side].local > 1) {
            WINDOW *nm1 = ((side == 0) ? subwnd1 : subwnd2);
            row = 30;
            wclear(nm1);

            int len_up = strlen(list[side].path);
            for (int i = list[side].local - 30; i < list[side].local; i++) {
              strcat(list[side].path, list[side].str[i]);
              DIR *drup = opendir(list[side].path);
              if (drup == NULL) {
                wattron(nm1, COLOR_PAIR(1));
                wattron(nm1, A_BOLD);
                wprintw(nm1, "%s\n", list[side].str[i]);
                wattron(nm1, A_BOLD);
                wattroff(nm1, COLOR_PAIR(1));
                wrefresh(nm1);
              } else {
                wattron(nm1, COLOR_PAIR(2));
                wattron(nm1, A_BOLD);
                wprintw(nm1, "%s\n", list[side].str[i]);
                wattroff(nm1, A_BOLD);
                wattroff(nm1, COLOR_PAIR(2));
                wrefresh(nm1);
                closedir(drup);
              }
              list[side].path[len_up] = '\0';
            }
            list[side].local--;
          } else {
            flash();
          }
        }

      } else if (c == KEY_DOWN) {
        if (list[side].poz - 1 < 30) {
          if (row < list[side].poz - 1) {
            row++;
            list[side].local++;
          }
        } else {
          if (row < 30 && list[side].local < list[side].poz - 1) {
            row++;
            list[side].local++;
          } else {
            WINDOW *nm = ((side == 0) ? subwnd1 : subwnd2);
            if (list[side].local < list[side].poz - 1) {
              row = 1;
              list[side].local++;
              wclear(nm);

              int len_down = strlen(list[side].path);
              for (int i = list[side].local;
                   i < (((list[side].poz) > list[side].local + 30)
                            ? list[side].local + 30
                            : (list[side].poz));
                   i++) {
                strcat(list[side].path, list[side].str[i]);
                DIR *drdw = opendir(list[side].path);
                if (drdw == NULL) {
                  wattron(nm, COLOR_PAIR(1));
                  wattron(nm, A_BOLD);
                  wprintw(nm, "%s\n", list[side].str[i]);
                  wattron(nm, A_BOLD);
                  wattroff(nm, COLOR_PAIR(1));
                  wrefresh(nm);
                } else {
                  wattron(nm, COLOR_PAIR(2));
                  wattron(nm, A_BOLD);
                  wprintw(nm, "%s\n", list[side].str[i]);
                  wattroff(nm, A_BOLD);
                  wattroff(nm, COLOR_PAIR(2));
                  wrefresh(nm);
                  closedir(drdw);
                }
                list[side].path[len_down] = '\0';
              }
            } else {
              flash();
            }
          }
        }
      } else if (c == '\n') {
        struct dirent **namelist3 = NULL;
        WINDOW *name = ((side == 0) ? subwnd1 : subwnd2);
        int slen = strlen(list[side].path);

        strcat(list[side].path, list[side].str[list[side].local]);

        int count = scandir(list[side].path, &namelist3, 0, alphasort);
        if (opendir(list[side].path) == NULL) {
          flag = 1;
          glob = slen;
          glob_side = side;
          glob_cols = cols;
          break;
        } else {
          if (count > 0) {
            list[side].poz = 1;

            wclear(name);

            int tlen = strlen(list[side].path);
            for (int i = 1; i < ((count > 31) ? 31 : count); i++) {
              strcat(list[side].path, "/");
              strcat(list[side].path, namelist3[i]->d_name);
              DIR *drcp = opendir(list[side].path);
              if (drcp == NULL) {
                wattron(name, COLOR_PAIR(1));
                wattron(name, A_BOLD);
                wprintw(name, "%s\n", namelist3[i]->d_name);
                wattron(name, A_BOLD);
                wattroff(name, COLOR_PAIR(1));
                wrefresh(name);
              } else {
                wattron(name, COLOR_PAIR(2));
                wattron(name, A_BOLD);
                wprintw(name, "%s\n", namelist3[i]->d_name);
                wattroff(name, A_BOLD);
                wattroff(name, COLOR_PAIR(2));

                wrefresh(name);
                closedir(drcp);
              }
              list[side].path[tlen] = '\0';
            }

            for (int i = 1; i < count; i++) {
              strcpy(list[side].str[list[side].poz], namelist3[i]->d_name);
              list[side].poz++;
              free(namelist3[i]);
            }
            free(namelist3);

            row = 1;
            list[side].local = 1;
            strcat(list[side].path, "/");
          }
        }
      }
    }
    delwin(wnd1);
    delwin(subwnd1);
    delwin(wnd2);
    delwin(subwnd2);
    endwin();

    if (flag == 1) {
      int status;
      pid_t pid = fork();
      if (pid == 0) {
        if (execl(list[side].path, list[side].path, NULL) == -1) {
          execl("/usr/bin/vim", "vim", list[side].path, NULL);
        }
      } else {
        wait(&status);
        printf("\n");
        printf("\033[92mTap 'ENTER' to continue...\033[39m\n"); 
        getch();
        system("clear");
        list[side].path[glob] = '\0';
      }
    }
  }
}
