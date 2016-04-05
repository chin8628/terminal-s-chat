#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "global_table.h"

//#include "command.h"

#define LENGHT_USERNAME 255
#define LENGHT_MESSAGE 255;

/*
// This function will handle connection for each client in each thread
*/

void *connection_handler (void *socket_desc) {

    int sock = *(int *)socket_desc;
    int receiver_sock, read_size;
    int i, j;
    char *message, client_message[LENGHT_MESSAGE], username[LENGHT_USERNAME], another[LENGHT_USERNAME];

    //Receive username and add address of sock's var into Global Table Socket
    do {
        read_size = recv(sock, username, LENGHT_USERNAME, 0);
    } while (read_size <= 0);
    add_user(&sock, username);

    do {
        read_size = recv(sock, receiver_sock, LENGHT_USERNAME, 0);
    } while (read_size <= 0);
    receiver_sock -= 48;

    //Receive a mssg from client
    while ( (read_size = recv(sock, client_message, LENGHT_MESSAGE, 0)) > 0 ) {

        //Check if get command from user
        if (client_message[0] == '/') {

            //Todo: send command from user to command's library
            //call_command(client_message);

        }
        else {

            //Echo mssg to client destination
            client_message[read_size] = '\0';
            printf("Recv from %d to send %d\n", sock, client_message, receiver_sock);
            write(receiver_sock, client_message, read_size);

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

        //Reply to the client
        message = "Hello Client , We was already connected together!\n";
        write(new_socket , message , strlen(message));

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
