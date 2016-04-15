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
#include "chat_library.h"

#define LENGHT_USERNAME 255
#define LENGHT_MESSAGE 256

/*
// This function will handle connection for each client in each thread
*/

void *connection_handler (void *socket_desc) {

    int sock = *(int *)socket_desc;
    int receiver_sock = -1, read_size, count_connection = 0;
    int i, j, error;
    char message[LENGHT_MESSAGE], client_message[LENGHT_MESSAGE];
    char buffer_message[LENGHT_MESSAGE];
    char username[LENGHT_USERNAME], another[LENGHT_USERNAME];

    //Reply to the client
    strcpy(message, "0server>> Hello Client , We was already connected together!\n");
    strcat(message, "server>> Let introduce yourself, What's your nickname?");
    write(sock ,message ,strlen(message));

    //Receive username and add address of sock's var into Global Table Socket
    do{

        read_size = recv(sock, buffer_message, LENGHT_USERNAME, 0);
        split_str(1, strlen(buffer_message), buffer_message, username);
        username[read_size] = '\0';

        error = add_user(username, sock);
        if (error == 101) {
            sprintf(message, "0server>> Nickname must lesser than 20 characters.");
        }
        else if (error == 102) {
            sprintf(message, "0server>> Name error.");
        }
        else {
            sprintf(message, "2server>> Your nickname is %s.", username);
        }
        write(sock, message, strlen(message));


    }while(error != 0);

    //Receive a mssg from client
    while ( (read_size = recv(sock, buffer_message, LENGHT_MESSAGE, 0)) > 0 ) {

        buffer_message[read_size] = '\0';
        split_str(1, strlen(buffer_message), buffer_message, client_message);

        //Check if get command from user
        if (client_message[0] == '/') {

            if (split_strcmp(0, 6, client_message, 0, 6, "/talkto")) {

                split_str(8, strlen(client_message), client_message, buffer_message);
                i = find_contact_by_user(buffer_message);
                if (i == -1) {
                    strcpy(message, "0server>> This nickname isn't exist.");
                }
                else {
                    receiver_sock = i;
                    sprintf(message, "0server>> Initial chat room with %s - %d", buffer_message, receiver_sock);
                }

                write(sock, message, strlen(message));

            }
            else if (split_strcmp(0, 6, client_message, 0, 6, "/untalk")) {
                receiver_sock = -1;
                sprintf(message, "0server>> Untalk is successful.");
                write(sock, message, strlen(message));
            }
            else if (split_strcmp(0, 7, client_message, 0, 7, "/contact")) {
                struct node *current;
            	current = root;
                sprintf(message, "0server>> ----------- User List -----------\n");
                strcat(message, "server>> ");
                i = 0;
            	while(current->next != 0){
            		current = current->next;
            		strcat(message, current->username);
                    strcat(message, " | ");
                    if (i == 3) {
                        strcat(message, "\n         ");
                        i = -1;
                    }
                    i++;
            	}
                write(sock, message, strlen(message));
            }

        }
        else {

            if (receiver_sock == -1) {

                strcpy(message, "0server>> Please /talkto for enter nickname who you want to chat.");
                write(sock, message, strlen(message));

            }
            else if (buffer_message[0] == '0'){

                //Echo mssg to client destination
                printf("Recv from %d to send %d\n", sock, receiver_sock);
                sprintf(message, "%c%s>> %s", buffer_message[0], username, client_message);
                write(receiver_sock, message, strlen(message));

            }
            else {

                //Send file to another client
                printf("Recv from %d to send %d\n", sock, receiver_sock);
                write(receiver_sock, buffer_message, strlen(buffer_message));

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

    pop_contact(username);

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

    return 0;
}
