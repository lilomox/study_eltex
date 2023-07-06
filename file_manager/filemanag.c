#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH_LEN 250
#define NAME_SIZE 20
#define LEN 30


void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

typedef struct List{
	int poz;
	char str[LEN][NAME_SIZE];
	char path[MAX_PATH_LEN];
}List;

int main()
{
	List list[2];
	list[0].poz = 1;
	list[1].poz = 1;
	strcpy(list[0].path, "./");
	strcpy(list[1].path, "./");

	initscr();
	signal(SIGWINCH, sig_winch);
	idlok(stdscr, TRUE);
	keypad(stdscr, TRUE);
	WINDOW *wnd1, *wnd2;
	WINDOW *subwnd1, *subwnd2;

	int row, cols;
	row = 1;
	cols = 1;

	refresh();

	noecho();
	wnd1 = newwin(32, COLS/2, 0, 0);
	box(wnd1, '|','=');
	subwnd1 = derwin(wnd1, 30, (COLS/2)-2, 1, 1);
	wrefresh(wnd1);

	wnd2 = newwin(32, COLS/2, 0, COLS/2);
	box(wnd2, '|','=');
	subwnd2 = derwin(wnd2, 30, (COLS/2)-2, 1, 1);
	wrefresh(wnd2);

	struct dirent **namelist1;
	struct dirent **namelist2;

	int n1, n2;

	n1 = scandir(list[0].path, &namelist1, 0, alphasort);
	if (n1 < 0) {
		goto to;
	} else {
		for (int i = n1-1; i > 0; i--) {
			wprintw(subwnd1, "%s\n", namelist1[i]->d_name);
			wrefresh(subwnd1);
			strcpy(list[0].str[list[0].poz], namelist1[i]->d_name);
			list[0].poz++;
	    free(namelist1[i]);
		}
	  free(namelist1);
	}

	n2 = scandir(list[1].path, &namelist2, 0, alphasort);
	if (n2 < 0) {
		goto to;
	} else {
		for (int i = n2-1; i > 0; i--) {
			wprintw(subwnd2, "%s\n", namelist2[i]->d_name);
			wrefresh(subwnd2);
			strcpy(list[1].str[list[1].poz], namelist2[i]->d_name);
			list[1].poz++;
	    free(namelist2[i]);
		}
	  free(namelist2);
	}
	
	int side = 0;

	while (1) {

		move(row, cols);
		wchar_t c = getch();

		
		if(c == '~') {
			goto to;
		} else if (c == KEY_BTAB) { 
			if (side == 0) {
				cols = (COLS/2)+1;
				side = 1;
			} else {
				cols = 1;
				side = 0;
			}
			row = 1;
		} else if (c == KEY_UP) {
      if (row > 1) {
        row--;
      } else {
        flash();
      }
    } else if (c == KEY_DOWN) {
      if (row < list[side].poz-1) {
        row++;
      } else {
        flash();
      }
    }
    
    if (c == '\n') {
      struct dirent **namelist3 = NULL;
      WINDOW *name = ((side == 0) ? subwnd1 : subwnd2);
      int slen = strlen(list[side].path);
      strcat(list[side].path, list[side].str[row]);
      
      int count = scandir(list[side].path, &namelist3, 0, alphasort);
      if (opendir(list[side].path) == NULL) {
      	flash();
      	list[side].path[slen] = '\0';
      	goto up;
      }
      
      if (count > 0) {
      	list[side].poz = 1;
      	
      	wclear(name);
      	for (int i = count-1; i > 0; i--) {
					wprintw(name, "%s\n", namelist3[i]->d_name);
					wrefresh(name);
					strcpy(list[side].str[list[side].poz], namelist3[i]->d_name);
					list[side].poz++;

					free(namelist3[i]);
				}
				
	    	free(namelist3);
	    	row = 1;
      	strcat(list[side].path, "/");

			} else {
				continue;
			}
    }
    up:
	}
	to:
	delwin(wnd1);
	delwin(subwnd1);
	endwin();
}