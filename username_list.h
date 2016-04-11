//global variable assignment
struct node {
	char username[20];
	int socket;
	struct node *next; //next address node linked
};
struct node *root;


int add_contact(char *name, int socket); // addcontact prototype
int pop_contact(char *name); // pop prototype
int display_user_list(); // read prototype
int find_contact(char *name); // find prototype
int contact_amount();
struct node* initial_username_listed();

struct node* initial_username_listed(){

	//Initial username's linked list
	root = malloc(sizeof(struct node));
	root->next = 0;
	return root;

}

int add_contact(char *name, int socket){

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

int pop_contact(char *name){
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

int display_user_list(){
	struct node *current;
	current = root;
	printf("----------- User List -----------\n");
	while(current->next != 0){
		current = current->next;
		printf("%04d: %s\n", current->socket , current->username);
	}
	printf("---------------------------------\n");
}

int find_contact_by_user(char *name){
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

const char * find_contact_by_socket(int socket){
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
