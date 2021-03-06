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
#include "global_search.h"

#define LENGHT_USERNAME 255
#define LENGHT_MESSAGE 255

WINDOW *global_typing;
WINDOW *global_display;
pthread_t *global_typing_thread, *global_display_thread;
int state = 0, display_height;
char username[LENGHT_USERNAME] = "";

void *display_func();
void *typing_func();

void* typing_func(void) {

    char message_buffer[LENGHT_MESSAGE];
    char message_buffer_2[LENGHT_MESSAGE];
    char confirm_file[LENGHT_MESSAGE];
    char filename[LENGHT_MESSAGE];
    char ch;
    int buffer_int;
    FILE *fp;

    while (state == 0) {

        //Reset string for get new message
        strcpy(message_buffer, "");
        strcpy(message_buffer_2, "");

        wscanw(global_typing, " %[^\n]s", message_buffer);
        while (strlen(message_buffer) > 200) {
            werase(global_typing);
            draw_new(global_display, "system>> Message cannot more than 200 characters.");
            wscanw(global_typing, " %[^\n]s", message_buffer);
        }

        //Draw_new line to display message
        strcpy(message_buffer_2, "you>> ");
        strcat(message_buffer_2, message_buffer);
        draw_new(global_display, message_buffer_2);


        //Check exit command
        if (strcmp(message_buffer, ":q!") == 0) {
            //set state to stop all function
            state = 1;
        }
        else if (message_buffer[0] == '/') {

            if (split_strcmp(0, 6, "/upload", 0, 6, message_buffer)){

                split_str(8, strlen(message_buffer), message_buffer, filename);
                sprintf(message_buffer, "3system>> Sending file to you: %s", filename);
                send_data(message_buffer);

                sleep(1);

                draw_new(global_display, "system>> Uploading...");

                fp = fopen(filename, "r");
                while( ( ch = fgetc(fp) ) != EOF ){

                    sprintf(message_buffer, "4%c", ch);

                    if(send_data(message_buffer) == 0)
                        draw_new(global_display, "system>> Send failed");

                }
                fclose(fp);

                sleep(1);

                strcpy(message_buffer, "5");
                send_data(message_buffer);
                draw_new(global_display, "system>> Done!");

            }
            else if (split_strcmp(0, 2, "/up", 0, 2, message_buffer)){

                split_str(4, strlen(message_buffer), message_buffer, message_buffer_2);
                buffer_int = atoi(message_buffer_2);
                draw_old_line(global_display, 1, buffer_int);

            }
            else if (split_strcmp(0, 4, "/down", 0, 4, message_buffer)){

                split_str(6, strlen(message_buffer), message_buffer, message_buffer_2);
                buffer_int = atoi(message_buffer_2);
                draw_old_line(global_display, 2, buffer_int);

            }
            else if (split_strcmp(0, 4, "/help", 0, 4, message_buffer)){

                draw_new(global_display, "system>> ### THIS IS HELP! ###");
                draw_new(global_display, "system>> \":q!\" to exit program.");
                draw_new(global_display, "system>> \"/talkto [nickname]\" to choose contact.");
                draw_new(global_display, "system>> \"/untalk\" to remove contact that we are talking.");
                draw_new(global_display, "system>> \"/upload [file]\" to upload file to client that you are talking.");
                draw_new(global_display, "system>> \"/watline\" to show number of latest line");
                draw_new(global_display, "system>> \"/up [amount of line]\" to scroll screen up n lines.");
                draw_new(global_display, "system>> \"/down [amount of line]\" to scroll screen down n lines.");
                draw_new(global_display, "system>> \"/find [word]\" to find number of line that word was display.");
                draw_new(global_display, "system>> \"/contact\" to show all user on server.");

            }
            else if (split_strcmp(0, 4, "/find", 0, 4, message_buffer)){

                split_str(6, strlen(message_buffer) - 1, message_buffer, message_buffer_2);
                search(message_buffer_2, global_display);

            }
            else if (split_strcmp(0, 7, "/watline", 0, 7, message_buffer)){

                //bottom_line come from buffer_screen.h
                sprintf(message_buffer, "system>> v This is lines number %d. v", bottom_line);
                draw_new(global_display, message_buffer);

            }
            else if (
                    split_strcmp(0, 6, "/talkto", 0, 6, message_buffer) ||
                    split_strcmp(0, 6, "/untalk", 0, 6, message_buffer) ||
                    split_strcmp(0, 7, "/contact", 0, 7, message_buffer)) {

                sprintf(message_buffer_2, "0%s", message_buffer);
                send_data(message_buffer_2);
            }
            else {

                draw_new(global_display, "system>> Command not found.");

            }
        }
        else {

            //Set protocal to send packet
            sprintf(message_buffer_2, "0%s", message_buffer);
            if(send_data(message_buffer_2) == 0)
                draw_new(global_display, "system>> Send failed");

        }

        werase(global_typing);

    }

    pthread_cancel(*global_display_thread);
    return 0;

}

void* display_func(void) {

    int i, j, error;
    char message_buffer[LENGHT_MESSAGE], message_buffer_2[LENGHT_MESSAGE];
    char filename[LENGHT_MESSAGE];
    char confirm_file[LENGHT_MESSAGE];
    FILE *fp;

    while (state == 0) {

        if (recieve_data(LENGHT_MESSAGE, message_buffer) == 0)
            draw_new(global_display, "system>> recieve error");

        //draw_new(global_display, message_buffer);
        if (message_buffer[0] == '0') {
            split_str(1, strlen(message_buffer), message_buffer, message_buffer_2);
            draw_new(global_display, message_buffer_2);
        }
        else if (message_buffer[0] == '1') {
            //do nothing
        }
        else if (message_buffer[0] == '2') {
            split_str(27, strlen(message_buffer) - 2, message_buffer, message_buffer_2);
            strcpy(username, message_buffer_2);
            split_str(1, strlen(message_buffer), message_buffer, message_buffer_2);
            draw_new(global_display, message_buffer_2);
        }
        else if (message_buffer[0] == '3') {
            split_str(1, strlen(message_buffer), message_buffer, message_buffer_2);
            draw_new(global_display, message_buffer_2);
            split_str(31, strlen(message_buffer) - 1, message_buffer, filename);

            sprintf(message_buffer_2, "downloaded/%s", filename);
            fp = fopen(message_buffer_2, "w+");
            error = 1;
            do {
                recieve_file(message_buffer);
                fprintf(fp, "%c", message_buffer[1]);
            }while(message_buffer[0] != '5');
            fclose(fp);

            draw_new(global_display, "system>> Downloaded file success.");

        }

        //Reset value in message_buffer for check while loop's condition
        strcpy(message_buffer, "");
        strcpy(message_buffer_2, "");

    }

    return 0;

}

int main(int argc , char *argv[]) {

    int read_size, port;
    char ip_address[255];
    char message_buffer[LENGHT_MESSAGE], c;
    char message_buffer_2[LENGHT_MESSAGE];
    WINDOW *buffer_window[2];

    //Config ip address and port
    strcpy(ip_address, argv[1]);
    port = atoi(argv[2]);

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

    //Initial connection server - client
    initial_connection(ip_address, port);

    //prepare to pthread_create with WINDOW *buffer_window[2];
    pthread_t typing_thread, display_thread;
    global_typing_thread = &typing_thread;
    global_display_thread = &display_thread;
    pthread_create( &typing_thread, NULL, (void *)typing_func, NULL);
    pthread_create( &display_thread, NULL, (void *)display_func, NULL );

    pthread_join( typing_thread, NULL);
    pthread_join( display_thread, NULL);

    // while (state == 0) {
    //     //do nothing
    // }

    draw_new(display, "\n------------------------------");
    draw_new(display, "Good bye, see you again! owo)/\n");
    sleep(2);

    // clean up
    delwin(display);
    delwin(typing);
    endwin();

    return 0;
}
