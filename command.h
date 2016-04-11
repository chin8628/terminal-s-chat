int read_user(); // read all user.
int add_user(char *name, int socket); // add a user.
int list_help(); // list all command
int lib_tester(char const *input); // testing purpose.
int check_name(char *name);

/* read_user function below is for listing all user in room.
   using read function in username_list.h */

int read_user(){
    display_user_list();
    return 0;
}

/* check_name function below is for checking condition before adding user,
   such as less than 20 characters, and doesn't duplicate with existing user. */

int check_name(char *name){
    int passable_name=1, duplicate=0, i=0;
    while(passable_name != 0){                                              // Loop check name.
        if(strlen(name) >= 20){                                             // Check if name has more than 20 chars.
            printf("Please enter lesser than 20 characters.\n");
            printf("Enter username: ");
            scanf(" %[^\n]s", name);
        }
        else {
            for(i=0; i<contact_amount(root); i++){
                if(strcmp(name, find_contact_by_socket(i)) == 0){     // Check if name is duplicated.
                    printf("Name error.\n");
                    printf("Enter username: ");
                    scanf(" %[^\n]s", name);
                    duplicate = 1;
                    break;
                }
                else{
                    duplicate = 0;
                }
            }
            if(duplicate != 1){                                             // Add user after all case are true.
                passable_name = 0;
            }
        }
    }
}

/* add_user function below is for... adding user to the room. Yeah.
   Using check_name in command.h and add_contact with read in username_list.h
*/

int add_user(char *name, int socket){
    check_name(name);
    add_contact(name, socket);
    display_user_list();
    return 0;
}

/* list_help function below is for listing all command available. */

int list_help(){
    printf("Command list:\n");
    printf("  /adduser (Add new user.)\n");
    printf("  /listuser (List all user in chat.)\n");
    printf("  /help (Show this message.)\n");
    return 0;
}

/* lib_tester function below is for testing this library. */

int lib_tester(char const *input) {
    struct node *root;
    root = malloc(sizeof(struct node));
    root->next = 0;
    if(strcmp(input, "/listuser") == 0){
        read_user(root);
    }
    else if(strcmp(input, "/adduser") == 0){
        char name[255];
        printf("Please enter a name: ");
        scanf("%s", name);
        add_user(name, 1);
    }
    else {
        list_help();
    }
    return 0;
}
