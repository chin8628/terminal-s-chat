#include <stdio.h>
#include <string.h>
#include "connection_clent.h"
#include <header.h>

#define LENGHT_MESSAGE 255

int main(int argc , char *argv[]) {

    int read_size;
    char message_buffer[LENGHT_MESSAGE], c;

    initial_connection("127.0.0.1", 8888);

    puts("\n-------------------------------");
    puts("   Welcome to terminal chat!");
    puts("-------------------------------\n");

    recieve_data(LENGHT_MESSAGE, message_buffer);
    puts(message_buffer);

    printf("Username: ");
    scanf(" %[^\n]s", message_buffer);
    if(send_data(message_buffer))
        puts("Send failed");

    printf("Chat with: ");
    scanf(" %[^\n]s", message_buffer);
    if(send_data(message_buffer))
        puts("Send failed");

    //Send some data
    do {
        //Reset value in message_buffer for check while loop's condition
        strcpy(message_buffer, "");

        //Send
        printf("You > ");
        scanf(" %[^\n]s", message_buffer);
        if(send_data(message_buffer))
            puts("Send failed");

        //Receive a reply from the server
        recieve_data(LENGHT_MESSAGE, message_buffer);
        puts(message_buffer);

    } while(strcmp(message_buffer, ":q!") != 0);

    puts("\n------------------------------");
    puts("Good bye, see you again! owo)/\n");

    return 0;
}
