#pragma once
#include<stdlib.h>
#include<stdio.h>
#include "types.h"

#ifndef LinkedList
#define LinkedList
struct List {
	struct List* next;
	struct List* prev;
	Object item;
};
typedef struct List* List;

List newList() {
	List list = malloc(sizeof(struct List));
	list->next = NULL;
	list->prev = NULL;
	list->item = NULL;
	return list;
}

void listAdd(List list, Object item) {
	if (list->item) {
		while(list->next) {
			list = list->next;
		}
		List node = malloc(sizeof(struct List));
		node->item = item;
		node->next = NULL;
		node->prev = list;
		list->next = node;
	} else {
		list->item = item;
	}
}

Object listGetN(List list, int index) {
 if (index < 0) return NULL;
	while(list->prev) {
		list = list->prev;
	}
	
	for (int i = 0; i < index; i++) {
		if (!list->next) return NULL;
		list = list->next;
	}
	
	return list;
}

Object listGet(List list, int index) {
	List node = listGetN(list, index);
	return node->item;
}

Object *listToArr(List list) {
	int size = 0;
	List node = list;
	while(node) {
		size += sizeof(node->item);
		node = node->next;
	}
	
	Object* arr = malloc(size);
	node = list;

	for (int i = 0; node; i++) {
		arr[i] = node->item;
		node = node->next;
	}
	return arr;
}

char** listToSArr(List list) {
	int size = 0;
	List node = list;
	int i = 0;
	while(node) {
//		printf("a%i - (%i) %s\n", i, sizeof(node->item), node->item);
		size += sizeof(node->item);
		node = node->next;
		i++;
	}
//	printf("Size: %i\n", size);
	fflush(stdout);
	
	char** arr = malloc(size);
	
	node = list;
	for (int i = 0; node; i++) {
		arr[i] = (char*) node->item;
		node = node->next;
	}
	return arr;
}

int *listToIArr(List list) {
	int size = 0;
	List node = list;
	while(node) {
		size += sizeof(node->item);
		node = node->next;
	}
	
	int* arr = malloc(size);
	
	node = list;
	for (int i = 0; node; i++) {
		arr[i] = (int*)node->item;
		node = node->next;
	}
	return arr;
}

//Kind of a mess, but it works
void listRem(List* list, int index) {
	List node = listGetN(*list, index);
	if (!node) return;
	
	if (node->prev)
		node->prev->next = node->next;
		else *list = (*list)->next;
	
	if (node->next) node->next->prev = node->prev;
	free(node);
}

void listDel(List list) {
	List node = listGetN(list, 0);
	while (node->next) {
		node = node->next;
		free(node->prev);
	}
	free(node);
}

int listSize(List list) {
	List node = list;
	int i;
	
	for (i = 0; node; i++) node = node->next;
	return i;
}
#endif
