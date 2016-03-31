#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurse.h>

#define LENLINE 1000

//global variable assignment
int line_amount = 0;
struct node {
	char string[LENLINE];
	struct node *next; //next and previous address node linked
};

int draw_new(char *name, int socket,struct node *root); // addcontact prototype
int pop_contact(char *name, struct node *root); // pop prototype
int read(struct node *root); // read prototype

int main(){

	//Initial username's linked list
	struct node *root, *current;
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;

	//Initial ncurse


}

int add_line(char *string, struct node *current) {

	// create new node
	current->next = malloc(sizeof(struct node));
	current = current->next;
	current->next = 0;

	//Copy input string into string's var inside node
	strcpy(current->string, string);
	line_amount++;

	return current;

}

int draw_new(char *string, struct node *current){

	//Add new line into node-linked list
	add_line(string, current);

	//Print string on screen by addstr (ncurse command)
	addstr(string);
	refresh();

	return 0;

}

int draw_old_line(struct node *current){

	//Not finished
	//Wait for idea
	refresh();
	return 0;

}
