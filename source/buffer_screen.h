#define LENLINE 500 //define limit lenght of each one line

//global variable assignment
struct node {
	int line_number;
	char string[LENLINE];
	struct node *next; //next and previous address node linked
};
struct node *current, *screen, *root;
int line_amount = 0, bottom_line = 0;
int display_height, typing_height;

void add_line(char *string);
void draw_new(WINDOW *display, char *string);
void draw_old_line(WINDOW *display, int option, int line_user_want_scroll);

void initial_buffer_screen(int dis_h, int ty_h) {

	display_height = dis_h;
	typing_height = ty_h;

	//Initial username's linked list
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;

}

// int example_test(){
//
// 	//Initial username's linked list
// 	root = malloc(sizeof(struct node));
// 	root->next = 0;
// 	current = root;
//
// 	char buffer[LENLINE];
// 	int parent_x, parent_y;
// 	int i, ch;
//
// 	//Initial ncurse
// 	initscr();
// 	curs_set(FALSE);
//
// 	// get our maximum window dimensions
// 	getmaxyx(stdscr, parent_y, parent_x);
//
// 	keypad(stdscr, TRUE);
// 	scrollok(stdscr, 1);
//
// 	//Print testing message
// 	for (i=1;i<=50;i++) {
// 		sprintf(buffer, "Testing scroll on number: %d", i);
// 		draw_new(stdscr, buffer);
// 	}
// 	refresh();
//
// 	//Get Arrow's key from keyboard for scroll up/down
// 	while ((ch = getch()) && ch != 'q')
// 	{
// 		if (ch == KEY_UP)
// 			draw_old_line(stdscr, parent_y, 1);
// 		else if (ch == KEY_DOWN )
// 			draw_old_line(stdscr, parent_y, 2);
// 	}
//
// 	refresh();
// 	endwin();
//
// 	return 0;
//
// }

void add_line(char *string){

	//move pointer to last node
	while(current->next != 0) current = current->next;

	// create new node
	current->next = malloc(sizeof(struct node));
	current = current->next;
	current->next = 0;

	//Copy input string into string's var inside node
	strcpy(current->string, string);

	//Assign line_number to node buffer
	current->line_number = line_amount;

}

void draw_new(WINDOW *display, char *string){

	int i;

	if (bottom_line < line_amount) {
		draw_old_line(display, 2, line_amount - bottom_line - 1);
		wprintw(display, "\n");
	}

	//Add new line into node-linked list
	add_line(string);

	//Print string on screen by addstr (ncurse command)
	screen = current;
	line_amount++;

	for (i=0;i<strlen(string);i++) {
		if (string[i] == '\n') line_amount += 1;
	}

	bottom_line = line_amount;

	//Print string on screen and refresh screen
	wprintw(display, "%s\n", string);
	wrefresh(display);

}

void draw_old_line(WINDOW *display, int option, int line_user_want_scroll){

	/*
		This function is for print line that is not display on screen
		In other hand, this function is for print string that is over-screen

		#let know "option"
		option is for say function that you will scroll up or down.
		option: 1 is scroll up.
		option: 2 is scroll down.
	*/

	int desc_line, i;

	for (i=0;i<line_user_want_scroll;i++) {
		//Set destination of line that program have to read for display from node
		if (option == 1) desc_line = bottom_line - display_height;
		else if (option == 2) desc_line = bottom_line + 1;

		//Initial current pointer for search in linked list
		current = root;
		while (current->next != 0 && current->next->line_number <= desc_line) current = current->next;

		/*
			### This comment is for if-else below ###

			- This section is for move screen up or down and print string from
			buffer list.
			- move(int y, int x) use to move cursor for print to top-left of screen
			to print first line.
			- FOR if: Move screen up and print string from buffer-list
			- FOR else if: Move screen down and print string from buffer-list
		*/


		if (option == 1 && bottom_line >= display_height){
			wscrl(display, -1);
			mvwprintw(display, 0, 0, "%s\n", current->string);
			bottom_line--;
		}
		else if (option == 2 && bottom_line < line_amount) {
			wscrl(display, 1);
			wmove(display, display_height - 1, 0);
			wprintw(display, "%s", current->string);
			bottom_line++;
		}
	}

	wrefresh(display);

}
