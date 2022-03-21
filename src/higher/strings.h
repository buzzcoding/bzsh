#pragma once
#include "types.h"
#include "list.h"

#include<string.h>
#include<stdio.h>
typedef const char* string;

/*int sarrlen(char** str) {
	int i;
	printf("%i, %i\n", 
			abs(str[1]) - abs(str[0]),
			sizeof(str[0]) * 8
	);
	for (i = 0; abs(str[i + 1]) - abs(str[i]) == sizeof(str[i]) * 8; i++) { printf("%i - %i - %s\n", i, str[i], str[i]); }
	i++;
	return i;
}*/

//Resizes string pointer to fit the actual size of the string
char* sfit(char *str) {
	char* newS = malloc(strlen(str) * sizeof(str[0]));
	strcpy(newS, str);
	free(str);
	str = newS;
	
	return str;
}

void checklist(List list);
List sfind(char *str, char *find) {
	List found = newList();
	if (!find || !strlen(find)) return found;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == find[0]) {
			bool isit = true;
			int j = 1;
			for (; j < strlen(find) && i + j < strlen(str); j++) {
				if (str[i + j] != find[j]) {
					isit = false;
					break;
				}
			}
			if (isit) {
				listAdd(found, (void*) (long) i);
				i += j - 1;
				continue;
			}
		}
	}
	return found;
}
void checklist(List list) {
	for (; list; list = list->next) {
		printf("%i:\n", list);
		printf("	next: %i\n", list->next);
		printf("	prev: %i\n", list->prev);
		printf("	item: %i\n", list->item);
		fflush(stdout);
	}
}

char *sreplace(char *str, char *replace, char *with) {
	List parts = newList();
	if (!replace || !strlen(replace)) return str;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == replace[0]) {
			bool isit = true;
			int j = 1;
			for (; j < strlen(replace) && i + j < strlen(str); j++) {
				if (str[i + j] != replace[j]) {
					isit = false;
					break;
				}
			}
			if (isit) {
				i += j - 1;
				if (strcmp(with, "")) listAdd(parts, with);
				continue;
			}
		}
		char* c = malloc(16);
		c[0] = str[i];
		c[1] = '\0';
		listAdd(parts, c);
	}
	
	int size = 0;
	List list = parts;
	while(list) {
		size += sizeof(list->item);
		list = list->next;
	}
	char* newstr = malloc(size);
	newstr[0] = '\0';
	
	list = parts;
	while(list) {
		strcat(newstr, list->item);
		list = list->next;
	}
	
	listDel(parts);
	return newstr;
}

List ssplit(char *str, char *split) {
	List parts = newList();
	if (!split || !strlen(split)) {
		for (int i = 0; i < strlen(str); i++) {
			char* tmp = malloc(sizeof(str[i]) * 2);
			tmp[0] = str[i];
			tmp[1] = '\0';
			listAdd(parts, tmp);
		}
		return parts;
	}
	int i, Pi, ssize = strlen(str);
	char* part = malloc(ssize * sizeof(str[0]));
	
	for (i = 0, Pi = 0; i < strlen(str); i++, Pi++) {
		if (str[i] == split[0]) {
			bool isit = true;
			int j = 1;
			for (; j < strlen(split) && i + j < strlen(str); j++) {
				if (str[i + j] != split[j]) {
					isit = false;
					break;
				}
			}
			if (isit) {
				part[Pi] = '\0';
				listAdd(parts, sfit(part));
				part = malloc(ssize * sizeof(str[0]));
				
				Pi = -1;
				i += j - 1;
				continue;
			}
		}
		part[Pi] = str[i];
	}
	
	part[Pi] = '\0';
	listAdd(parts, sfit(part));
	return parts;
}

char* sjoin(int len, char** arr, char* delim) {
	if (!arr || !len) return NULL;
	
	int size = 0;
	for (int i = 0; i < len; i++) {
		size += strlen(arr[i]) * sizeof(arr[i][0]);
		if (i + 1 == len) size += strlen(delim) * sizeof(delim[0]);
	}
	
	char* newS = malloc(size + sizeof(char));
	newS[0] = '\0';

	for (int i = 0; i < len - 1; i++) {
		strcat(newS, arr[i]);
		strcat(newS, delim);
	}
	strcat(newS, arr[len - 1]);
	strcat(newS, "\0");
	return sfit(newS);
}
