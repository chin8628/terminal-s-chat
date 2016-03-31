#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define LENLINE 1000

//global variable assignment
struct node {
	int line_number;
	char string[LENLINE];
	struct node *next; //next and previous address node linked
};
struct node *current, *screen, *root;
int line_amount = 0, bottom_line = 0;

int add_line(char *string);
int draw_new(char *string);
int draw_old_line(int height_screen, int option);

int main(){

	//Initial username's linked list
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;

	//Initial ncurse
	int parent_x, parent_y;
	int new_x, new_y;
	int score_size = 3;
	int i;
	char buffer[LENLINE];

	initscr();
	curs_set(FALSE);

	// get our maximum window dimensions
	getmaxyx(stdscr, parent_y, parent_x);

	// set up initial windows
	WINDOW *area = newwin(parent_y - score_size, parent_x, 0, 0);

	keypad(stdscr, TRUE);
	scrollok(stdscr, 1);

	for (i=0;i<100;i++) {
		sprintf(buffer, "Testing scroll on number: %d\n", i);
		draw_new(buffer);
	}
	refresh();

	int ch;
	while ((ch = getch()) && ch != 'q')
	{
		if (ch == KEY_UP)
		{
			scrl(-1);
			draw_old_line(parent_y, 1);
		}
		else if (ch == KEY_DOWN )
		{
			scrl(1);
			draw_old_line(parent_y, 2);
		}
	}

	refresh();
	endwin();

	return 0;

}

int add_line(char *string) {

	// create new node
	current->next = malloc(sizeof(struct node));
	current = current->next;
	current->next = 0;

	//Copy input string into string's var inside node
	strcpy(current->string, string);

	//Assign line_number to node buffer
	current->line_number = line_amount;

	return 0;

}

int draw_new(char *string){

	//Add new line into node-linked list
	add_line(string);

	//Print string on screen by addstr (ncurse command)
	screen = current;
	line_amount++;
	bottom_line = line_amount;

	//Print string on screen and refresh screen
	printw(screen->string);
	refresh();

	return 0;

}

int draw_old_line(int height_screen, int option){

	//option: 1 is scroll up, 2 is scroll down

	int desc_line;
	char buffer[10];

	if (option == 1) desc_line = bottom_line - height_screen;
	else if (option == 2) desc_line = bottom_line + 1;

	current = root;

	if (height_screen > 0) {

		while (current->line_number < desc_line) current = current->next;

		sprintf(buffer, "%d\n", bottom_line);

		if (option == 1){
			mvprintw(0, 0, current->string);
			bottom_line--;
		}
		else if (option == 2) {
			mvprintw(height_screen - 1, 0, current->string);
			bottom_line++;
		}

	}

	refresh();

	return 0;

}
