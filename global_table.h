struct socket_table {
    char username[1000];
    int *socket;
};

struct socket_table global_socket[10];
int count_connection = 0;

int add_user (int *sock, char *username);
int number_connection();
char *show_username(int order);

char *show_username(int order) {
    //Wait linked list jame
}

int number_connection() {
    return count_connection;
}

int add_user (int *sock, char *username) {

    //Input: address of socket in connection_handler()
    //Input: username's string

    count_connection++;
    global_socket[count_connection].socket = sock;
    strcpy(global_socket[count_connection].username, username);
}
