#include <stdio.h>
#include <stdlib.h>
#include "username_list.h"
#include <string.h>

struct node *root;

int main() {
    int i=0;
    char input[255];
    struct node *root, *current;
    current = root;
    root = malloc(sizeof(struct node));
    root->next = 0;
    while(1){
        scanf(" %s", input);
        if(strcmp(input, "/listuser") == 0){                                        // List all user(s).
            read(root);
        }
        else if(strcmp(input, "/adduser") == 0){                                    // Add user case.
            int passable_name=1, duplicate=0;
            char name[255];
            printf("Enter username: ");
            scanf(" %[^\n]s", name);
            while(passable_name != 0){                                              // Loop check name.
                if(strlen(name) >= 20){                                             // Check if name is more than 20 chars.
                    printf("Please enter lesser than 20 characters.\n");
                    printf("Enter username: ");
                    scanf(" %[^\n]s", name);
                }
                else {
                    for(i=0; i<contact_amount(root); i++){
                        if(strcmp(name, find_contact_by_socket(i, root)) == 0){     // Check if name is duplicated.
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
                        add_contact(name, contact_amount(root), root);
                        read(root);
                    }
                }
            }
        }
        else if(strcmp(input, "/help") == 0){                                       // List of command implimented.
            printf("Command list:\n");
            printf("  /adduser (Add new user)\n");
            printf("  /listuser (List all user in chat)\n");
        }
    }
    return 0;
}
