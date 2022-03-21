#pragma once
#include<limits.h>

#ifdef _WIN32
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
int setenv(const char *name, const char *value, int overwrite);
#endif

char *username();
char *hostname();
char *homedir();
unsigned char getchB();
unsigned char getcheB();
void tRmChar();
int runBin(char* cmd, char* argv[]);