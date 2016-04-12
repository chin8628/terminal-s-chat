#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connection_clent.h"
#include "buffer_screen.h"

#define LENGHT_MESSAGE 255

WINDOW *global_typing;
WINDOW *global_display;

void *display();
void *typing();

void* display_func(void) {

    char message_buffer[LENGHT_MESSAGE];
    wscanw(global_typing, " %[^\n]s", message_buffer);
    if(send_data(message_buffer) == 0)
        draw_new(global_display, "system>> Send failed");
    werase(global_typing);

}

void* typing_func(void) {

    char message_buffer[LENGHT_MESSAGE];

    //Reset value in message_buffer for check while loop's condition
    strcpy(message_buffer, "");

    recieve_data(LENGHT_MESSAGE, message_buffer);
    draw_new(global_display, message_buffer);

}

int main(int argc , char *argv[]) {

    int read_size;
    char message_buffer[LENGHT_MESSAGE], c;
    WINDOW *buffer_window[2];


    /////////////////////////////////////////////////
    //             START NCURSES CODE              //
    /////////////////////////////////////////////////

    //Set variable about ncurses
    int parent_x, parent_y;
    int typing_size = 4;

    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_RED);

    // get our maximum window dimensions
    getmaxyx(stdscr, parent_y, parent_x);

    // set up initial windows
    WINDOW *display = newwin(parent_y - typing_size - 1, parent_x, 0, 0);
    WINDOW *split_line = newwin(1, parent_x, parent_y - typing_size - 1, 0);
    WINDOW *typing = newwin(typing_size, parent_x, parent_y - typing_size, 0);

    global_typing = typing;
    global_display = display;

    scrollok(display, 1);

    //wbkgd(display, COLOR_PAIR(2));
    //wrefresh(display);

    wbkgd(split_line, COLOR_PAIR(1));
    wrefresh(split_line);

    //wbkgd(typing, COLOR_PAIR(2));
    //wrefresh(typing);

    initial_buffer_screen();

    /////////////////////////////////////////////////
    //               END NCURSES CODE              //
    /////////////////////////////////////////////////

    draw_new(display, "--------------------------------------------------");
    draw_new(display, "             Welcome to terminal chat!            ");
    draw_new(display, "--------------------------------------------------\n");

    draw_new(display, "system>> Terminal-chat is started.");

    //Initial connection server - client
    initial_connection("127.0.0.1", 8888);

    recieve_data(LENGHT_MESSAGE, message_buffer);
    draw_new(display, message_buffer);

    draw_new(display, "system>> Please enter Nickname.");
    wscanw(typing, " %[^\n]s", message_buffer);
    werase(typing);
    if(send_data(message_buffer) != 0)
        draw_new(display, "system>> Send failed");
    recieve_data(LENGHT_MESSAGE, message_buffer);
    draw_new(display, message_buffer);
    werase(typing);

    //prepare to pthread_create with WINDOW *buffer_window[2];
    pthread_t typing_thread, display_thread;
    pthread_create( &typing_thread, NULL, (void *)typing_func, NULL);
    pthread_create( &display_thread, NULL, (void *)display_func, NULL );

    draw_new(display, "\n------------------------------");
    draw_new(display, "Good bye, see you again! owo)/\n");

    // clean up
    delwin(display);
    delwin(typing);
    endwin();

    return 0;
}
