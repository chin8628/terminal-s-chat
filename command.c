int talk_one();
int list_user();

int talk_one () {
    //Input contant that client want to talk
    do {
        read_size = recv(sock, another, 1000, 0);
        puts(another);
    } while (read_size <= 0);

    for (i=0;i<count_connection;i++) {
        if (strcmp(another, global_socket[i].username) == 0) {
            printf("%s %s", another, global_socket[i].username);
            receiver_sock = *global_socket[i].socket;
            puts("Found chat contact!");
            break;
        }
    }
}
