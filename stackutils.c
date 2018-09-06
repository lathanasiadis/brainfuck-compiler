#include <stdio.h>
#include <stdlib.h>
#include "stackutils.h"

/*init a new stack.
returns 0 if malloc fails and 1 otherwise*/
int stack_init(Stack **head){
		
	*head = malloc(sizeof(Stack));
	if (*head == NULL)
		return 0;
	
	(*head)->next = NULL;
		
	return 1;
}

int stack_is_empty(Stack *head){
	if (head->next == NULL)
		return 1;
		
	return 0;
}

/*push a new val to the stack*/
int push(Stack **head,int val){
	Stack *new,*cur;
	
	new = malloc(sizeof(Stack));
	
	new->val = val;
	new->next = NULL;
	
	if (new == NULL)
		return 0;
	
	if ( (*head)->next == NULL )
		(*head)->next = new;
	else{
		for (cur = (*head)->next; cur->next != NULL; cur = cur->next);
		cur->next = new;
	}
	
	return 1;
}

/*pop(remove) the latest node
returns 0 if pop failed(because the stack was empty),
otherwise 1*/
int pop(Stack **head){
	Stack *prev,*cur;

	if (stack_is_empty(*head))
		return 0;
	
	for (cur = (*head)->next, prev=(*head); cur->next != NULL; cur = cur->next, prev=prev->next);
	
	free(cur);
	prev->next = NULL;
		
	return 1;
}

/*Return the latest node without removing it.
Returns NULL if the stack is empty*/
Stack *top(Stack *head){
	Stack *cur;
	
	if (stack_is_empty(head))
		return NULL;
	
	for (cur = head->next; cur->next != NULL; cur = cur->next);
	return cur;
}

/*print the stack as if the stack is growing from top to bottom*/
void print(Stack *head){
	Stack *cur;
	
	for (cur=head->next; cur!=NULL; cur=cur->next){
		printf("%d\n",cur->val);
	}
}

void nuke_stack(Stack **head){
	free(*head);
}
