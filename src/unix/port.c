#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<pwd.h>

#include "../port.h"
#include "getch.h"

pid_t pid;

//void signaltochild(int sig) {
//	kill(pid, sig);
//}

char *username() {
    return getpwuid(geteuid())->pw_name;
}

char *hostname() {
    char* name = malloc(256);
    gethostname(name, 256);
    return name;
}

char *homedir() {
	return getpwuid(geteuid())->pw_dir;
}

unsigned char getchB() {
	unsigned char gotch = _getch();
	return gotch;
}

unsigned char getcheB() {
	unsigned char gotch = _getche();
	return gotch;
}

void tRmChar() {
	printf("\33[1D\33[K");
}

int runBin(char* cmd, char* argv[]) {
	int status;

	pid = fork();
//	signal(SIGINT, signaltochild);
//	signal(SIGSTOP, signaltochild);
//	signal(SIGQUIT, signaltochild);
//	signal(SIGTSTP, signaltochild);
	
	if (!pid) {
		exit(execvp(cmd, argv));
	} else {
		wait(&status);
		pid = 0;
		return WEXITSTATUS(status);
	}
}
