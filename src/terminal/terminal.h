#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

//#include "../../../higherc/src/higherc.h"
#include "higherc.h"
#include "../port.h"

#define argmax 524288
#define pthmax 65535
#define envmax 32767
struct List* lines;
char pwd[envmax];
int latest = 0;
int lineN = 0;
int debug = 0;
char* user;
char* host;

char *parsePS1(char* PS1) {
	;char newS[envmax] = "";
	for(int i = 0; i < envmax; i++) {
		if (PS1[i] == '\0') break;
		if (PS1[i] == '\\') {
			switch(PS1[i + 1]) {
				case 'H':
					strcat(newS, host);
					break;
				case 'u':
					strcat(newS, user);
					break;
				case 'w':
					strcat(newS, pwd);
					break;
				default:
					;char tmp[2] = { PS1[i] };
					strcat(newS, tmp);
					continue;
			}
			i++;
			
		} else {
			char tmp[2] = { PS1[i] };
			strcat(newS, tmp);
		}
	}
	
	char* newP = malloc(envmax * sizeof(char));
	strcpy(newP, newS);
	
	return newP;
}

char* getPS1() {
	return parsePS1(getenv("PS1"));
}

void resetTerm() {
	printf("\033c");
}

void clearTerm() {
	printf("\033[2J");
}

char *readLine() {
	//if (latest) printf("%i - %s\n", latest - 1, listGet(lines, latest - 1));
	char* PS1 = getPS1();
	printf("%s", PS1);
	fflush(stdout);

	char* line = malloc(argmax * sizeof(char));
	char* tmpline = line;
	int i = 0;
	
	while (1) {
		unsigned char a = getchB();
		
		if (a == 26 || a == 28 || a == 25) {
			if (debug) {
				printf("\n");
				fflush(stdout);
				exit(0);
			}
			continue;
		}
		//printf("%c", a);
		
		switch(a) {
			case 3:
				printf("\n");
				return NULL;
			case '\r':
			case '\n':
				printf("\n");
				line[i] = '\0';
				//if (!strcmp(line, "exit")) exit(0);
				listAdd(lines, line);
				break;
			case 12:
				clearTerm();
				//clearTerm();
				printf("\33[2K\r%s%s", PS1, line);
				//for (int j = pos; j <= i; j++) printf("\010");
				fflush(stdout);
				continue;
			case 8:
			case 127:
				if (i < 1) continue;
				i--;
				line[i] = '\0';
				tRmChar();
				fflush(stdout);
				continue;
			case 224:
			case 27:
				;unsigned char b = getchB();
				if (b == 91 || a == 224) {
					unsigned char c = (a == 224) ? b : getchB();
					switch(c) {
						case 72:
						case 65:
							if (lineN < 1) break;
							//char* tmp = malloc(argmax * sizeof(char));
							if (tmpline == line) line = malloc(argmax * sizeof(char));
							lineN--;
							//printf("\n%s\n", listGet(lines, lineN));
							strcpy(line, listGet(lines, lineN));
							i = strlen(line);
							printf("\33[2K\r%s%s", getPS1(), line);
							fflush(stdout);
							break;
						case 80:
						case 66:
							if (lineN >= latest) break;
							lineN++;
							
							if (lineN == latest) line = tmpline;
							else strcpy(line, listGet(lines, lineN));
							i = strlen(line);
							
							printf("\33[2K\r%s%s", getPS1(), line);
							fflush(stdout);
							break;
						/*case 68:
							if (pos < 1) break;
							pos--;
							printf("\010");
							fflush(stdout);
							break;
						default:
							printf(" %i ", c);
							fflush(stdout);*/
					}
				}
				continue;
			default:
				printf("%c", a);
				/*if (pos < i) {
					for (int j = pos; j < i; j++)
						printf("%c", line[j]);
					for (int j = pos; j < i; j++)
						line[j + 1] = line[j];
				}*/
				fflush(stdout);
				line[i] = a;
				i++;
				continue;
		}
		break;
		//printf("\n%i - %s\n", i, line);
	}
	//printf("%i - %s\n", latest, listGet(lines, latest));
	latest++;
	lineN = latest;
	free(PS1);
	return line;
}
