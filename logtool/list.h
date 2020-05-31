
#include <stdlib.h>
#include <string.h>


#ifndef LIST_C
#define LIST_C

typedef struct ListElmt_{
	void *data;
	struct ListElmt_ *next;
} ListElement;
typedef struct List_{
	int size;
	int (*math)(const void *key1,const void *key2);
	void (*destroy)(void *data);
	ListElement *head;
	ListElement *tail;
} List;

void list_init(List *list,void (*destroy)(void *data));
void list_destroy(List *list);
int  list_insert_next(List *list,ListElement *element, const void *data);
int  list_remove_next(List *list,ListElement *element,void **data);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_is_head(list,element) (element==(list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)


#endif