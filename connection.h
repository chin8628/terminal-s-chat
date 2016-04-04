int initial_connection(char *ip_addr, int port) {

    int socket_desc;
    struct sockaddr_in server;

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
