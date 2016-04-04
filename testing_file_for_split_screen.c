// demo.c

#include <ncurses.h>
#include <unistd.h>

void draw_borders(WINDOW *screen) {
    int x, y, i; getmaxyx(screen, y, x);

    // 4 corners
    mvwprintw(screen, 0, 0, "+");
    mvwprintw(screen, y - 1, 0, "+");
    mvwprintw(screen, 0, x - 1, "+");
    mvwprintw(screen, y - 1, x - 1, "+");

    // sides
    for (i = 1; i < (y - 1); i++) {
        mvwprintw(screen, i, 0, "|");
        mvwprintw(screen, i, x - 1, "|");
    }

    // top and bottom
    for (i = 1; i < (x - 1); i++) {
        mvwprintw(screen, 0, i, "-");
        mvwprintw(screen, y - 1, i, "-");
    }
}

int main(int argc, char *argv[]) {
    int parent_x, parent_y;
    int score_size = 4;

    initscr();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);

    // get our maximum window dimensions
    getmaxyx(stdscr, parent_y, parent_x);

    // set up initial windows
    WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);
    WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);

    //wattron(field, COLOR_PAIR(1));
    whline(score, ACS_HLINE, parent_x);
    //wattroff(field, COLOR_PAIR(1));

    // draw to our windows
    mvwprintw(field, 0, 1, " Field ");
    mvwprintw(score, 0, 1, " Score ");

    // refresh each window
    wrefresh(field);
    wrefresh(score);

    char input[255];
    mvwscanw(score, 1, 0, "%s", input);

    // clean up
    delwin(field);
    delwin(score);
    endwin();

    return 0;
}
