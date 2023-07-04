#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#define SIZE 30
#define NAME_SIZE 20
#define MAX_STRING_SIZE 72

void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

int main() {
  initscr();
  signal(SIGWINCH, sig_winch);
  idlok(stdscr, TRUE);
  keypad(stdscr, TRUE);
  WINDOW *wnd;
  WINDOW *subwnd;
  while (1) {
    refresh();

    curs_set(0);
    noecho();

    wnd = newwin(7, 67, 2, 4);
    box(wnd, '|', '-');
    subwnd = derwin(wnd, 5, 65, 1, 1);

    wprintw(
        subwnd,
        "  Hello, User, this is a simple text editor! I have 2 commands:\n \n"
        "\t\t1)edit file \t 2)exit\n");

    wprintw(subwnd, "\n");

    wprintw(subwnd, "Enter num of comand: ");

    wattron(subwnd, A_BLINK);
    wprintw(subwnd, "...\n");
    wattroff(subwnd, A_BLINK);

    wrefresh(wnd);

    delwin(subwnd);
    delwin(wnd);

    char c = getch();

    if (c == '2') {
      break;
    } else if (c == '1') {
      clear();

      echo();
      char string[NAME_SIZE];

      attron(A_BOLD);
      move(1, 4);
      printw(
          "I can view the file and edit it. To close the file without saving "
          "enter '~'. To close the file and save the changes type '@'.\n");
      attroff(A_BOLD);

      attron(A_BOLD);
      move(7, 16);
      printw("Enter path to file");
      attron(A_BLINK);
      printw(": \n");
      attroff(A_BLINK);
      attroff(A_BOLD);

      refresh();
      move(7, 36);
      scanw("%s", string);

      FILE *file = fopen(string, "r+");
      if (file == NULL) {
        move(9, 16);
        printw("No such of file\n");

        attron(A_BLINK);
        move(10, 16);
        printw("Press any key to continue...");
        attroff(A_BLINK);

        getch();

        refresh();
        clear();
      } else {
        clear();

        noecho();
        curs_set(1);

        char arr[MAX_STRING_SIZE];
        while (fgets(arr, MAX_STRING_SIZE, file) != NULL) {
          printw("%s", arr);
        }
        fclose(file);
        refresh();

        wchar_t t;
        int row, cols;
        row = 0;
        cols = 0;

        while (1) {
          move(row, cols);
          refresh();

          t = getch();

          switch (t) {
          case '~':
            goto to;
            break;
          case KEY_LEFT:
            if (cols > 0) {
              cols--;
            } else {
              flash();
            }
            break;
          case KEY_RIGHT:
            if (cols < COLS - 1) {
              cols++;
            } else {
              flash();
            }
            break;
          case KEY_UP:
            if (row > 0) {
              row--;
            } else {
              flash();
            }
            break;
          case KEY_DOWN:
            if (row < LINES - 1) {
              row++;
            } else {
              flash();
            }
            break;
          case KEY_BACKSPACE:
            cols--;
            move(row, cols);
            delch();
            break;
          case '@':
            remove(string);
            FILE *new_file = fopen(string, "w");
            for (int l = 0; l < 23; l++) {
              for (int i = 0; i < 75; i++)
                putc(mvinch(l, i) & A_CHARTEXT, new_file);
              putc('\n', new_file);
            }

            fclose(new_file);
            goto to;
          default:
            move(row, cols++);
            insch(t);
          }
        }
      to:

        clear();
      }

    } else {
      move(0, 5);

      attron(A_BOLD);
      printw("You wrote not a command\n");
      attroff(A_BOLD);

      attron(A_BLINK);
      move(1, 5);
      printw("- - - - - - - - - - - -\n");
      attroff(A_BLINK);

      refresh();
    }
  }

  endwin();
  exit(EXIT_SUCCESS);
}
