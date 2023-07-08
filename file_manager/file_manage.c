#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH_LEN 30000
#define NAME_SIZE 70
#define LEN 30000

void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

typedef struct List{
	int poz;
	int local;
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

	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);

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

	int n;
	int tl = strlen(list[1].path);
	n = scandir(list[1].path, &namelist1, 0, alphasort);

	for (int i = 1; i < ((n > 31) ? 31 : n); i++) {
		strcat(list[1].path, namelist1[i]->d_name);
		DIR *drst = opendir(list[1].path);
		if (drst == NULL) {
			wattron(subwnd1, COLOR_PAIR(1));
			wattron(subwnd2, COLOR_PAIR(1));

			wattron(subwnd1, A_BOLD);
			wattron(subwnd2, A_BOLD);

			wprintw(subwnd1, "%s\n", namelist1[i]->d_name);
			wprintw(subwnd2, "%s\n", namelist1[i]->d_name);

			wattroff(subwnd1, A_BOLD);
			wattroff(subwnd2, A_BOLD);

			wattroff(subwnd1, COLOR_PAIR(1));
			wattroff(subwnd2, COLOR_PAIR(1));

			wrefresh(subwnd1);
			wrefresh(subwnd2);
		} else {
			wattron(subwnd1, COLOR_PAIR(2));
			wattron(subwnd2, COLOR_PAIR(2));

			wattron(subwnd1, A_BOLD);
			wattron(subwnd2, A_BOLD);

			wprintw(subwnd1, "%s\n", namelist1[i]->d_name);
			wprintw(subwnd2, "%s\n", namelist1[i]->d_name);

			wattroff(subwnd1, A_BOLD);
			wattroff(subwnd2, A_BOLD);

			wattroff(subwnd1, COLOR_PAIR(2));
			wattroff(subwnd2, COLOR_PAIR(2));

			wrefresh(subwnd1);
			wrefresh(subwnd2);
			closedir(drst);
		}
		list[1].path[tl] = '\0';
	}	
	
	for (int i = 1; i < n; i++) {
		strcpy(list[1].str[i], namelist1[i]->d_name);
		strcpy(list[0].str[i], namelist1[i]->d_name);
	  free(namelist1[i]);
	}
	free(namelist1);
	list[0].poz = list[1].poz = n;

	list[1].local = list[0].local = 1;
	int side = 0;

	wchar_t c;
	while (c != '~') {
		move(row, cols);
		c = getch();
		
		if(c == '~') {
			break;
		} else if (c == KEY_BTAB) { 
			if (side == 0) {
				cols = (COLS/2)+1;
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
      		for (int i = list[side].local-30; i < list[side].local; i++) {
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
      if (list[side].poz-1 < 30) {
      	if (row < list[side].poz-1) {
      		row++;
      		list[side].local++;
      	}
      } else {
      		if (row < 30 && list[side].local < list[side].poz-1) {
      			row++;
      			list[side].local++;
      		} else {
      			WINDOW *nm = ((side == 0) ? subwnd1 : subwnd2);
      			if (list[side].local < list[side].poz-1) {
      				row = 1;
      				list[side].local++;
      				wclear(nm);

      				int len_down = strlen(list[side].path);
      				for (int i = list[side].local; i < (((list[side].poz) > list[side].local+30) ? list[side].local+30 : (list[side].poz)); i++) {
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
      	flash();
      	list[side].path[slen] = '\0';
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
	endwin();
}