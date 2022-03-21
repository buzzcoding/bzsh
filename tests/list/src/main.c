#include<stdio.h>
#include "list.h"

int main() {
    struct List* list = newlist();
    listadd(list, "Bruh");
    listadd(list, "H");
    listadd(list, "A");
    printf("%s\n", listget(list, 2));
}
