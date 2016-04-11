#include "header.h"

#define LENGHT_MESSAGE 255

void *display(WINDOW *screen) {

}

void *typing(WINDOW *screen) {

}

int main(int argc , char *argv[]) {

    int read_size;
    char message_buffer[LENGHT_MESSAGE], c;


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

    scrollok(display, 1);

    //wbkgd(display, COLOR_PAIR(2));
    //wrefresh(display);

    wbkgd(split_line, COLOR_PAIR(1));
    wrefresh(split_line);

    //wbkgd(typing, COLOR_PAIR(2));
    //wrefresh(typing);

    initial_buffer_screen();

    char input[255];
    draw_new(display, "Hello");
    wscanw(typing, "%s", input);

    /////////////////////////////////////////////////
    //               END NCURSES CODE              //
    /////////////////////////////////////////////////


    //Initial connection server - client
    initial_connection("127.0.0.1", 8888);

    recieve_data(LENGHT_MESSAGE, message_buffer);
    puts(message_buffer);

    draw_new(display, "\n-------------------------------");
    draw_new(display, "   Welcome to terminal chat!");
    draw_new(display, "-------------------------------\n");

    draw_new(display, "Please enter Nickname.");
    wscanw(typing, " %[^\n]s", message_buffer);
    werase(typing);
    if(send_data(message_buffer))
        draw_new(display, "Send failed");

    draw_new(display, "Chat with: ");
    wscanw(typing, " %[^\n]s", message_buffer);
    werase(typing);
    if(send_data(message_buffer))
        draw_new(display, "Send failed");

    //Send some data
    do {
        werase(typing);

        //Reset value in message_buffer for check while loop's condition
        strcpy(message_buffer, "");

        //Send
        wscanw(typing, " %[^\n]s", message_buffer);
        if(send_data(message_buffer))
            draw_new(display, "Send failed");

        werase(typing);

        //Receive a reply from the server
        recieve_data(LENGHT_MESSAGE, message_buffer);
        draw_new(display, message_buffer);

    } while(strcmp(message_buffer, ":q!") != 0);

    draw_new(display, "\n------------------------------");
    draw_new(display, "Good bye, see you again! owo)/\n");

    // clean up
    delwin(display);
    delwin(typing);
    endwin();

    return 0;
}
