#ifndef __STACK_UTILS_H__
#define __STACK_UTILS_H__
#endif

typedef struct stack {
	int val;
	struct stack *next;
} Stack;

int stack_init(Stack **head);
int stack_is_empty(Stack *head);
int push(Stack **head,int val);
int pop(Stack **head);
Stack *top(Stack *head);
void print(Stack *head);
void nuke_stack(Stack **head);
