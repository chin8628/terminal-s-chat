// prototype insensitive case search
int search(char *word, WINDOW *display);

int search(char *word, WINDOW *display){
    struct node *current;
    current = root;
	int hit=0;
    //Line amout from buffer_screen.h
    int current_line = line_amount - 2;
    int count = 0;
    int i, j;
    char buffer[LENLINE];
    draw_new(display, "system>> ### This /find have error value at 5 line. ###");
	while(current->next != 0 && count < current_line){
		current = current->next;
		if(strstr(current->string, word) != 0){
            sprintf(buffer, "system>> Found at line: %d", current->line_number);
			draw_new(display, buffer);
			hit++;
		}
        count++;
	}
	if(hit==0){
		draw_new(display, "system>> Word not found.");
	}
	return -1;
}
