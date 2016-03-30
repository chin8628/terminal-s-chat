//Finished just draw_new function, Sleeping zzZZ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENLINE 255

//global variable assignment
struct node {
	char string[LENLINE];
	struct node *next; //next and previous address node linked
};

int line_amount = 0;

int draw_new(char *name, int socket,struct node *root); // addcontact prototype
int pop_contact(char *name, struct node *root); // pop prototype
int read(struct node *root); // read prototype

int main(){

	//Initial username's linked list
	struct node *root, *current;
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;

}

//------------------------------------------------------------------------------

int draw_new(char *string, struct node *root){

	int i = 0;
	struct node *current;

	current = root;
	while ( current->next != 0)
        current = current->next;

	//########## create new node ##########//

	current->next = malloc(sizeof(struct node));
	current = current->next;
	current->next = 0;

	//Copy input string into string's var inside node
	strcpy(current->string, string);

	line_amount++;

}

//-------------------------------------------------------------------------------

int pop_contact(char *name,struct node *root){
	struct node *current, *temp;
	current = root;

	while(current->next != 0){
		if(strcmp(current->next->username, name) == 0){
			temp = current->next;
			current->next = current->next->next;
			free(temp);				  // clear(pop) memory of disconect node.
			return 0;
		}
		current = current->next;
	}
}

//---------------------------------------------------------------------------------


int read(struct node *root){
	struct node *current;
	current = root;
	while(current->next != 0){
		current = current->next;
		printf("----------------------\n");
		printf("User : %s\nsocket : %d\n", current->username, current->socket);
		printf("----------------------\n");
	}
}
