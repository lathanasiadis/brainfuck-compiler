#ifndef __LIST_UTILS_H__
#define __LIST_UTILS_H__
#endif

typedef struct list {
	int value;
	struct list *next;
	struct list *prev;
} List;

int list_init(List **head);
int list_add(List **head,int value);
void print_list(List *head);
List *get_newest_node(List *head);
int rm_newest_node(List **head);
void nuke_list(List **head);
