#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "username_list.h"
#include "command.h"

#define LENGHT_USERNAME 255
#define LENGHT_MESSAGE 255

/*
// This function will handle connection for each client in each thread
*/

void *connection_handler (void *socket_desc) {

    int sock = *(int *)socket_desc;
    int receiver_sock = -1, read_size, count_connection = 0;
    int i, j, error;
    char message[LENGHT_MESSAGE], client_message[LENGHT_MESSAGE], username[LENGHT_USERNAME], another[LENGHT_USERNAME];
    char buffer_reciever[10];

    //Reply to the client
    strcpy(message, "server>> Hello Client , We was already connected together!");
    write(sock ,message ,strlen(message));

    //Receive username and add address of sock's var into Global Table Socket
    do {
        read_size = recv(sock, username, LENGHT_USERNAME, 0);
    } while (read_size <= 0);
    error = add_user(username, sock);
    if (error == 101) {
        sprintf(message, "server>> Nickname must lesser than 20 characters.");
    }
    else if (error == 102) {
        sprintf(message, "server>> Name error.");
    }
    else {
        sprintf(message, "server>> Your nickname is %s.", username);
    }
    write(sock, message, strlen(message));

    //Receive a mssg from client
    while ( (read_size = recv(sock, client_message, LENGHT_MESSAGE, 0)) > 0 ) {

        //Check if get command from user
        if (client_message[0] == '/') {

            if (strcmp(client_message, "/talkto") == 0) {

                do{
                    strcpy(message, "server>> Enter nickname who want to chat with.");
                    write(sock, message, strlen(message));
                    read_size = recv(sock, buffer_reciever, LENGHT_USERNAME, 0);
                }while(read_size <= 0);

                sprintf(message, "Initial chat room with %s", buffer_reciever);
                write(sock, message, strlen(message));

                receiver_sock = atoi(buffer_reciever);

            }

        }
        else {

            if (receiver_sock == -1) {

                strcpy(message, "server>> Please \\talkto for enter nickname who you want to chat.");
                write(sock, message, strlen(message));

            }
            else {

                //Echo mssg to client destination
                client_message[read_size] = '\0';
                printf("Recv from %d to send %d\n", sock, receiver_sock);
                write(receiver_sock, client_message, read_size);
            }

        }
    }

    count_connection -= 1;

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1) {
        puts("Receive failed");
    }

    //Free pointer of socket
    free(socket_desc);

    return 0;

}

int main (int argc, char *argv[]) {

    int socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;
    char *message;

    initial_username_listed();

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        puts("Could not create soclet");
    }

    //Prepare the sockaddr_in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind socket
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}
