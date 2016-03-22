#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc , char *argv[]) {
    int socket_desc, read_size;
    struct sockaddr_in server;
    char message[255] , server_reply[2000], c;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

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
