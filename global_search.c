#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "global_table.h"
#include "connection_clent.h"
#include "buffer_screen.h"
#include "username_list.h"

#define LENLINE 500 //define limit lenght of each one line

struct node{
  int line_number;
  char string[LENLINE];
  struct node *next;
};

struct node *current, *root;

int search(char *word);    // prototype insensitive case search

int main(){

	//example test
	root = malloc(sizeof(struct node));
	root->next = 0;
	current = root;


	return 0;
}


int search(char *word){
	int hit=0;
	while(current->next != 0){
		current = current->next;
		if(strstr(current->string, word) != 0){
			printf("  L: %d | [ %s ]\n",current->line_number,current->string);
			hit++;
		}
	}
	if(hit==0){
		printf("Word not found.");
	}
	return -1;
}
