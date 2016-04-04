/*
    This connection file is for client side only!!
*/


#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int socket_desc;
struct sockaddr_in server;

int initial_connection(char *ip_addr, int port) {

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(ip_addr); // set IP ADDRESS of server
    server.sin_family = AF_INET;
    server.sin_port = htons(port); // set port of chat service

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

}

int recieve_data(int lenght, char *message_buffer){

    char server_reply[lenght];
    int read_size;

    // recieve packet from server
    read_size = recv(socket_desc, server_reply , lenght , 0);

    if (read_size > 0) {
        /*
            set end point string on last character by calculate read_size
            In normal, recv don't clear last buffer string
            So we must set end of string to prevent bug from last buffer string
        */
        server_reply[read_size] = '\0';
        strcpy(message_buffer, server_reply);
        return 1;
    }
    else {
        puts("Receive failed.");
        return 0;
    }

}

int send_data(char *text) {

    return send(socket_desc , text , strlen(text) , 0) > 0;

}
