#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#include "terminal/terminal.h"
#include "port.h"

const char* shname = "bzsh";
const char* shvers = "alpha 0.x.x";
char* defPS1 = "[\\u@\\H \\w]$ ";
int running = 1;
int histstart = 0;

struct bzsh_cmd {
	int argc;
	char** cmd;
	char* argstr;
//	char* next;
	List cmdL;
};

#define tprintf(msg, format) \
	printf(msg, format); \
	fflush(stdout);

#define tprint(msg) \
	printf(msg); \
	fflush(stdout);

void handlectrlc(int sig) {
	signal(SIGINT, handlectrlc);
//	ctrlc = 1;
//	printf("\n%s", getenv("PS1"));
	fflush(stdout);
}

void handlesegfault(int sig) {
	printf("\nInternal shell error:\nSegmentation fault (%i)\n", sig);
	fflush(stdout);
	exit(sig);
}

void throwError(char* msg, int isfatal, int sherror) {
	if (msg && strcmp(msg, ""))
		if (sherror) printf("%s: %s\n", shname, msg);
		else printf("%s\n", msg);
	if (isfatal) running = 0;
}

/*char** arrRange(int Sarr, char** arr, int s, int e) {
	printf("A - %i, %i-%i\n", Sarr, s, e);
	fflush(stdout);
	char** newA = malloc(Sarr * sizeof(char*));
	for (int i = s, j = 0; i < Sarr && i <= e; i++, j++) {
		printf("%s\n", arr[i]);
		fflush(stdout);
		newA[j] = arr[i];
	}
	printf("B\n");
	fflush(stdout);

	return newA;
}*/

char* arrStr(int Sarr, char** arr) {
	printf("C\n");
	fflush(stdout);
	int size = 0;
	for (int i = 0; i < Sarr; i++) {
		printf("%s\n", arr[i]);
		size += sizeof(arr[i]);
	}

	printf("C.5 %i\n", size);
	fflush(stdout);	

	char* newS = malloc(size * sizeof(char));
	for (int i = 0; i < Sarr; i++) strcat(newS, arr[i]);

	printf("D\n");
	fflush(stdout);
	return newS;
}

void envset(char* name, char* value) {
//	char* var = malloc(envmax * sizeof(char));
//	sprintf(var, "%s=%s", name, value);
	setenv(name, value, 1);
}

void envsetinit(char* name, char* value) {
	setenv(name, value, 0);
}

int chkBuiltin(int* found, char* cmd, char* builtin) {
	int isit = !strcmp(cmd, builtin);
	if (isit) *found = 1;
	return isit;
}

//Will need to be partially or completely rewritten to implement things requiring multiple lines, like functions, and things running multiple commands, like &&, |, ||, and ;
struct bzsh_cmd* parsecmd(char* line) {
	//Ignore empty input
	if (!line) return NULL;
	int len = strlen(line);	
	if (len < 1) return NULL;
	if (line[0] == '\0') return NULL;
	int isblank = 1;
	for (int i = 0; i < len; i++) {
		if (!isspace(line[i])) isblank = 0;
	}
	if (isblank) return NULL;
	if (line[0] == '#') return NULL;

	
	List cmdL = ssplit(line, " ");
	char** cmd = (char**) listToSArr(cmdL);
	int argc = listSize(cmdL);
	char* argstr = sjoin(argc - 1, cmd + 1, " ");
	if (!argstr) {
		argstr = malloc(1);
		argstr[0] = '\0';
	}

	struct bzsh_cmd* cmdS = malloc(sizeof(struct bzsh_cmd));
	cmdS->cmdL = cmdL;
	cmdS->argc = argc;
	cmdS->cmd = cmd;
	cmdS->argstr = argstr;
	return cmdS;
}

void cleancmd(struct bzsh_cmd* cmdS) {
	if (!cmdS) return;
	listDel(cmdS->cmdL);
	free(cmdS->cmd);
	free(cmdS->argstr);
	free(cmdS);
}

void runcmd(struct bzsh_cmd* cmdS) {
	int argc = cmdS->argc;
	char** cmd = cmdS->cmd;
	char* argstr = cmdS->argstr;

	int found = 0;
	//printf("%s, %s\n", cmd[0], cmd[1]);
	//return;

	//Builtins
	if (chkBuiltin(&found, cmd[0], "exit")) running = 0;
	if (chkBuiltin(&found, cmd[0], "echo")) printf("%s\n", argstr);
	if (chkBuiltin(&found, cmd[0], "clear")) resetTerm();
	if (chkBuiltin(&found, cmd[0], "nextchar")) printf("%i\n", getchB());
	if (chkBuiltin(&found, cmd[0], "printenv")) printf("%s\n", getenv(argstr));
	if (chkBuiltin(&found, cmd[0], "parsePS1")) printf("%s\n", parsePS1(argstr));
	if (chkBuiltin(&found, cmd[0], "cd")) strcpy(pwd, argstr);
	if (chkBuiltin(&found, cmd[0], "export")) envset(cmd[1], sjoin(argc - 2, cmd + 2, " "));
	if (chkBuiltin(&found, cmd[0], "pwd")) printf("%s\n", pwd);
	if (chkBuiltin(&found, cmd[0], "error")) throwError(argstr, 0, 1);
	if (chkBuiltin(&found, cmd[0], "fatalerror")) throwError(argstr, 1, 1);
	
	//For checking in a script if the current bzsh version has more advanced features like if, while, functions, pipe, and mutli-command lines. Can be used to make a script not error out in lower versions, like this one.
	//Ex: hasnoadv fatalerror Please use bzsh version (insert version here) to use this script
	if (chkBuiltin(&found, cmd[0], "hasnoadv")) {
		struct bzsh_cmd* nacmd = parsecmd(argstr);
		if (!nacmd) return;
		runcmd(nacmd);
		cleancmd(nacmd);
	}
	if (found) return;

	//Executing commands
	int tmp = runBin(cmd[0], cmd);
	//printf("%i\n", tmp);
	if (tmp != 255) found = 1;
	
	if (!found) printf("%s: %s: command not found\n", shname, cmd[0]);
}

void runscript(FILE* file) {
	char* line;
	size_t len;
	while(getline(&line, &len, file) != -1) {
		char* linep = malloc(sizeof(line));
		strcpy(linep, line);
		struct bzsh_cmd* cmd = parsecmd(linep);
		if (!cmd) continue;
		runcmd(cmd);
		cleancmd(cmd);
	}
}


//printf("\x001b[38;5;10m");
int main(int argc, char* argv[]) {
	//Initialization
	{
		signal(SIGINT, handlectrlc);
		signal(SIGSEGV, handlesegfault);

		envsetinit("0", argv[0]);
		envsetinit("USER", user = username());
		envsetinit("HOSTNAME", host = hostname());
		envset("PS1", defPS1); //Change to envset later
		envset("PWD", homedir());
	
		strcpy(pwd, getenv("PWD"));

 		char histbuff[pthmax] = "";
		strcpy(histbuff, homedir());
		strcat(histbuff, "/");
		strcat(histbuff, ".bzsh_history");
		envsetinit("HISTFILE", histbuff);

		lines = newList();
 		FILE* hist = fopen(histbuff, "r");
		char* line;
		size_t len;
		while (getline(&line, &len, hist) != -1) {
			char* linep = malloc(sizeof(line));
			strcpy(linep, line);
			linep[strlen(linep) - 1] = '\0';
			listAdd(lines, linep);
			latest++;
		}
		lineN = latest;
		fclose(hist);
		List item;
		for (List line = item; line; line = line->next) { histstart++; }
		running = 1;
	}

	//Parse arguments
	if (argc > 1) {
		FILE *file;
		if (!strcmp(argv[1], "--version")) {
			printf("%s (%s) %s\n", argv[0], shname, shvers);
			running = 0;
		} else if ((file = fopen(argv[1], "r"))) {
			runscript(file);
			fclose(file);
		}
		running = 0;
	}
	
	//Tmp
	if (!running) {
//		struct List* list = newlist();
//		listadd(list, "Bruh");
//		listadd(list, "H");
//		printf("%s\n", listget(list, 1));
//		listadd(list, "A");
//		printf("%s\n", listget(list, 2));
//		while(1) {
//			
//		}
	}

	//Start command line
	while (running) {
		char* line = readLine();
		fflush(stdout);
		if (!running) break;

		struct bzsh_cmd* cmd = parsecmd(line);
		if (!cmd) continue;
		runcmd(cmd);
		cleancmd(cmd);
	}

	//Write history, clean up, and exit
 	printf("a\n");
	FILE *hist = fopen(getenv("HISTFILE"), "w");
	List tmp = listGetN(lines, histstart);
	for (List line = tmp; line; line = line->next) { fprintf(hist, "%s\n", (char*) line->item); }
	fclose(hist);
	listDel(lines);
	return 0;
}
