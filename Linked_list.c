#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//global variable assignment 
struct node {
	char username[20]; 
	int *socket;
	struct node *next, *prev; //next and previous address node linked 
};


//int add_contact(char *name, int *socket); // addcontact prototype

int main(){
	char name, *a_name;
	int socket, *a_socket;
	scanf("%s %d", name, &socket);
	a_name = &name;
	a_socket = &socket;
	add_contact(a_name, a_socket);
}



int add_contact(char *name, int *socket){
	int i=0, contact_amount=0;
	struct node *root, *current;

//########### set root node ############
	root = malloc(sizeof(struct node));
	root->next = 0;
	root->prev = 0;
	current = root;
	
//############ shift current node to end of list ###########
	while ( current->next != 0)
	{
        current = current->next;
	}
	
//########## create new node ##########	

	current->next = malloc(sizeof(struct node));
	current->next->prev = current;
	current = current->next;
	current->next = 0;
	strcpy(current->username, name);
	current->socket = socket; 
	contact_amount++;
			
// #########  read node  ##########
/*	current = root;
	while ( current->next != 0 )
    {
    	current = current->next;
    	printf("%s\n%d\n", current->username, current->socket);
	}	
*/
	
	
	
	
}











