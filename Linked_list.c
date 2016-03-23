#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global variable assignment 
struct node {
	char username[20]; 
	int socket;
	struct node *next; //next and previous address node linked 
};


int add_contact(char *name, int socket,struct node *root); // addcontact prototype
int pop_contact(char *name, struct node *root); // pop prototype
int read(struct node *root); // read prototype

int main(){
	int i,socket;
	char name[20], *a_name;
	struct node *root, *current;
	
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;
	for(i=0;i<5;i++){
	
		scanf("%s %d", name, &socket);
		a_name = name;
		add_contact(a_name, socket, root);
		
	}
	printf("Before pop contact\n");
	read(root);
	scanf("%s", name);
	pop_contact(name, root);
	printf("After pop contact\n");
	read(root);
}

//------------------------------------------------------------------------------

int add_contact(char *name, int socket, struct node *root){
	
	int i=0, contact_amount=0;
	struct node *current;

	current = root;
	while ( current->next != 0)
        current = current->next;
	
	//########## create new node ##########	

	current->next = malloc(sizeof(struct node));
	current = current->next;
	current->next = 0;
	
	strcpy(current->username, name);
	current->socket = socket; 
	
	contact_amount++;
					
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












