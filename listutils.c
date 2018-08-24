#include <stdio.h>
#include <stdlib.h>
#include "listutils.h"
/*init a new stack.returns 0 if malloc fails and 1 otherwise*/
int list_init(List **head){
		
	*head = malloc(sizeof(List));
	if (*head == NULL)
		return 0;
		
	(*head)->next = (*head);
	(*head)->prev = (*head);
		
	return 1;
}

int list_add(List **head,int value){
	List *new;
	
	new = malloc(sizeof(List));
	
	if (new == NULL)
		return 0;
	
	new->prev = *head;
	new->next = (*head)->next;
	
	new->prev->next = new;
	new->next->prev = new;
	
	new->value = value;
	
	return 1;
}

void print_list(List *head){
	List *cur;
	
	for (cur=head->next;cur!=head;cur=cur->next){
		printf("%d\n",cur->value);
	}

}

List *get_newest_node(List *head){
	return head->next;
}

int rm_newest_node(List **head){
	List *newest = (*head)->next;
	
	if (newest == *head)
		return 0;	
	
	newest->prev->next = newest->next;
	newest->next->prev = newest->prev;
	free(newest);
	return 1;
}

void nuke_list(List **head){
	free(*head);
}
