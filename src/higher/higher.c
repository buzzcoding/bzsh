#include "types.h"
#include "list.h"
#include "map.h"
#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)

#define foreachL(item, list) \
	for (List node = list, item = node->item; node; node = node->next, item = (node) ? node->item : NULL)

#include "strings.h"
