#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "global_table.h"
#include "connection_clent.h"
#include "buffer_screen.h"
#include "username_list.h"

void draw_borders(WINDOW *screen);

int main(int argc, char *argv[]) {

    int parent_x, parent_y;
    int new_x, new_y;
    int score_size = 3;
    int i;

    initscr();
    noecho();
    curs_set(FALSE);

    // get our maximum window dimensions
    getmaxyx(stdscr, parent_y, parent_x);

    // set up initial windows
    WINDOW *area = newwin(parent_y - score_size, parent_x, 0, 0);

    keypad(stdscr, TRUE);
    scrollok(stdscr, 1);

    for (i=0;i<1000;i++) {
        printw("Testing scroll on number: %d\n", i);
    }
    refresh();
    sleep(1);

    int ch;
    while ((ch = getch()) != EOF && ch != 'q')
    {
        if (ch == KEY_UP)
        {
            scrl(-1);
        }
        else if (ch == KEY_DOWN )
        {
            scrl(1);
        }
    }

    sleep(2);

    endwin();

    return 0;
}
