#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "connection.h"

#define LENGHT_MESSAGE 255

int main(int argc , char *argv[]) {
    int read_size;
    char message[LENGHT_MESSAGE] , server_reply[LENGHT_MESSAGE], c;

    initial_connection("127.0.0.1", 8888)

    puts("\n-------------------------------");
    puts("   Welcome to terminal chat!");
    puts("-------------------------------\n");

    read_size = recv(socket_desc, server_reply , 2000 , 0);
    server_reply[read_size] = '\0';
    printf("%s\n", server_reply);

    printf("Username: ");
    scanf(" %[^\n]s", message);
    if( send(socket_desc , message , strlen(message) , 0) < 0)
        puts("Send failed");

    printf("Chat with: ");
    scanf(" %[^\n]s", message);
    if( send(socket_desc , message , strlen(message) , 0) < 0)
        puts("Send failed");

    //Send some data
    do {
        //Reset value in message for check while loop's condition
        strcpy(message, "");

        //Send
        printf("You > ");
        scanf(" %[^\n]s", message);
        if( send(socket_desc , message , strlen(message) , 0) < 0)
            puts("Send failed");

        //Receive a reply from the server
        if( (read_size = recv(socket_desc, server_reply , 2000 , 0)) < 0)
            puts("recv failed");
        server_reply[read_size] = '\0';
        puts(server_reply);

    } while(strcmp(message, ":q!") != 0);

    puts("\n------------------------------");
    puts("Good bye, see you again! owo)/\n");

    return 0;
}
