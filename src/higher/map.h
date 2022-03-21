#pragma once
#include<stdlib.h>
#include "types.h"

#ifndef MapI
#define MapI
struct MapEntry {
	Object key;
	Object value;
};

struct Map {
	List list;
};
typedef struct MapEntry* MapEntry;
typedef struct Map* Map;

inline MapEntry newMapE(Object key, Object value) {
	MapEntry entry = malloc(sizeof(struct MapEntry));
	entry->key = key;
	entry->value = value;
	return entry;
}

inline Map newMap() {
	Map map = malloc(sizeof(struct Map));
	map->list = newList();
	return map;
}

inline List mapGetN(Map map, Object key) {
	List list = map->list;
	while(list && list->item) {
		MapEntry e = list->item;
		if (e->key == key) return list;
		list = list->next;
	}
	return NULL;
}

inline Object mapGet(Map map, Object key) {
	List list = mapGetN(map, key);
	if (!list || !list->item) return NULL;
	return ((MapEntry) list->item)->value;
}

inline Object mapGetI(Map map, int index) {
	List node = listGetN(map->list, index);
	return ((MapEntry) node->item)->value;
}

inline void mapSet(Map map, Object key, Object value) {
	List node;
	if (node = mapGetN(map, key))
		((MapEntry) node->item)->value = value;
	else
		listAdd(map->list, newMapE(key, value));
}

inline void mapRem(Map map, Object key) {
	
}

inline void mapDel(Map map) {
	List node = listGetN(map->list, 0);
	while (node->next) {
		node = node->next;
		free(node->prev->item);
		free(node->prev);
	}
	free(node->item);
	free(node);
	free(map);
}
#endif
