#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "buffer_screen.h"
#include "connection_clent.h"
#include "chat_library.h"

#define LENGHT_MESSAGE 255

WINDOW *global_typing;
WINDOW *global_display;
int state = 0, display_height;

void *display_func();
void *typing_func();

void* typing_func(void) {

    char message_buffer[LENGHT_MESSAGE];
    char message_buffer_2[LENGHT_MESSAGE];
    int buffer_int;

    while (state == 0) {

        //Reset string for get new message
        strcpy(message_buffer, "");
        strcpy(message_buffer_2, "");

        // if (ch == KEY_UP || ch == KEY_DOWN) {
        //     if (ch == KEY_UP){
        //     	draw_old_line(global_display, global_parent_y, 1);
        //     }
        //     else if (ch == KEY_DOWN) {
    	// 		draw_old_line(global_display, global_parent_y, 2);
        //     }
        // }

        wscanw(global_typing, " %[^\n]s", message_buffer);

        //Check exit command
        if (strcmp(message_buffer, ":q!") == 0) {
            //set state to stop all function
            state = 1;
            return 0;
        }
        else if (split_strcmp(0, 2, "/up", 0, 2, message_buffer)){
            split_str(4, strlen(message_buffer), message_buffer, message_buffer_2);
            buffer_int = atoi(message_buffer_2);
            draw_old_line(global_display, 1, buffer_int);
        }
        else {
            strcpy(message_buffer_2, "you>> ");
            strcat(message_buffer_2, message_buffer);
            draw_new(global_display, message_buffer_2);
            if(send_data(message_buffer) == 0)
                draw_new(global_display, "system>> Send failed");
        }

        werase(global_typing);

    }

}

void* display_func(void) {

    char message_buffer[LENGHT_MESSAGE];

    while (state == 0) {

        if (recieve_data(LENGHT_MESSAGE, message_buffer) == 0)
            draw_new(global_display, "system>> recieve error");

        draw_new(global_display, message_buffer);

        //Reset value in message_buffer for check while loop's condition
        strcpy(message_buffer, "");

    }

}

int main(int argc , char *argv[]) {

    int read_size;
    char message_buffer[LENGHT_MESSAGE], c;
    char message_buffer_2[LENGHT_MESSAGE];
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

    display_height = parent_y - typing_size - 1;

    global_typing = typing;
    global_display = display;

    scrollok(display, 1);

    //wbkgd(display, COLOR_PAIR(2));
    //wrefresh(display);

    wbkgd(split_line, COLOR_PAIR(1));
    wrefresh(split_line);

    //wbkgd(typing, COLOR_PAIR(2));
    //wrefresh(typing);

    initial_buffer_screen(display_height, typing_size);

    /////////////////////////////////////////////////
    //               END NCURSES CODE              //
    /////////////////////////////////////////////////

    draw_new(display, "--------------------------------------------------");
    draw_new(display, "             Welcome to terminal chat!            ");
    draw_new(display, "--------------------------------------------------\n");

    draw_new(display, "system>> Terminal-chat is started.");
    draw_new(display, "system>> type \":q!\" to exit program.");

    //Initial connection server - client
    initial_connection("127.0.0.1", 8888);

    recieve_data(LENGHT_MESSAGE, message_buffer);
    draw_new(display, message_buffer);

    //prepare to pthread_create with WINDOW *buffer_window[2];
    pthread_t typing_thread, display_thread;
    pthread_create( &typing_thread, NULL, (void *)typing_func, NULL);
    pthread_create( &display_thread, NULL, (void *)display_func, NULL );

    while (state == 0) {
        //do nothing
    }

    draw_new(display, "\n------------------------------");
    draw_new(display, "Good bye, see you again! owo)/\n");
    sleep(2);

    // clean up
    delwin(display);
    delwin(typing);
    endwin();

    return 0;
}
