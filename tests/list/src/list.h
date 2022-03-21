#pragma once
#include<stdlib.h>

#ifndef LinkedList
#define LinkedList
struct List {
	struct List* next;
	struct List* prev;
	void* item;
};

struct List *newlist() {
	struct List* list = malloc(sizeof(struct List));
	return list;
}

void listadd(struct List* list, void* item) {	
	if (list->item) {
		while(list->next) {
			list = list->next;
		}
		struct List *node = malloc(sizeof(struct List));
		node->item = item;
		node->next = NULL;
		node->prev = list;
		list->next = node;
	} else {
		list->item = item;
	}
}

void* listget(struct List* list, int index) {
	while(list->prev) {
		list = list->prev;
	}
	
	for (int i = 0; i < index; i++) {
		if (!list->next) return NULL;
		list = list->next;
	}
	
	return list->item;
}
#endif
