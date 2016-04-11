#include <header.h>

//global variable assignment
struct node {
	char username[20];
	int socket;
	struct node *next; //next address node linked
};


int add_contact(char *name, int socket,struct node *root); // addcontact prototype
int pop_contact(char *name, struct node *root); // pop prototype
int read(struct node *root); // read prototype
int find_contact(char *name, struct node *root); // find prototype
int contact_amount(struct node *root);

// int main(){
// 	int i,socket;
// 	char name[20], *a_name;
// 	struct node *root, *current;
//
// 	root = malloc(sizeof(struct node));
// 	root->next = 0;
// 	current = root;
// 	for(i=0;i<5;i++){
// 		scanf(" %[^\n]s", name);
// 		scanf(" %d", &socket);
// 		a_name = name;
// 		add_contact(a_name, socket, root);
//
// 	}
// 	printf("Before pop contact\n");
// 	read(root);
// 	printf("Contact in server : %d\n",contact_amount(root));
// 	printf("\n\nDisconect Contact : ");
// 	scanf(" %[^\n]s", name);
// 	pop_contact(name, root);
// 	printf("\nAfter pop contact\n");
// 	read(root);
// 	printf("Contact in server : %d\n",contact_amount(root));
//
// 	printf("\n----------- Test Find contact  -------------\n");
// 	scanf(" %[^\n]s", name);
// 	a_name = name;
// 	printf("Username : %s\nSocket : %d", name, find_contact(name,root));
// }

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
	printf("----------- User List -----------\n");
	while(current->next != 0){
		current = current->next;
		printf("%04d: %s\n", current->socket , current->username);
	}
	printf("---------------------------------\n");
}

// --------------------------------------------------------------------------------

int find_contact_by_user(char *name,struct node *root){
	struct node *current;
	current = root;
	while(current->next != 0){
		if(strcmp(current->next->username, name) == 0){
			return current->next->socket;
		}
		current = current->next;
	}
	return -1;
}

const char * find_contact_by_socket(int socket,struct node *root){
	struct node *current;
	current = root;
	while(current->next != 0){
		if(current->next->socket == socket){
			return current->next->username;
		}
		current = current->next;
	}
	return 0;
}

int contact_amount(struct node *root){
	struct node *current;
	int amount=0;
	current = root;
	while(current->next != 0){
		current = current->next;
		amount++;
	}
	return amount;
}
