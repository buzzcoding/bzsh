#include<windows.h>
#include<shlobj.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<errno.h>
#include<wchar.h>
#include<conio.h>

#include "../port.h"

#define _CRT_NONSTDC_NO_WARNINGS

char *username() {
	char* name = malloc(256);
	DWORD bufL = 256;
	GetUserNameA(name, &bufL); //Down to Windows 2000
	return name;
}

char *hostname() {
	char* name = malloc(256);
	DWORD bufL = 256;
	GetComputerNameA(name, &bufL); //Down to Windows 2000
	return name;
}

char *homedir() {
	WCHAR dirW[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, dirW); //Down to Windows 2000
	
	char* dir = malloc(MAX_PATH);
	wcstombs(dir, dirW, wcslen(dirW) + 1); //Down to Windows [Unknown]
	return dir;
}

unsigned char getchB() {
	unsigned char gotch = getch(); //Down to Windows [Unknown]
	return gotch;
}

unsigned char getcheB() {
	unsigned char gotch = getche(); //Down to Windows [Unknown]
	return gotch;
}

void tRmChar() {
	printf("\010 \010");
}

int runBin(char* cmd, char* argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
 
	int status = CreateProcess(argv[0], cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); //Down to Windows XP
	printf("%i ", status);
	
	WaitForSingleObject( pi.hProcess, INFINITE );
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	return status;
}

//All the code after this comment is yoninked

//https://stackoverflow.com/questions/735126/are-there-alternate-implementations-of-gnu-getline-interface/735472#735472
/* The original code is public domain -- Will Hartung 4/9/09 */
/* Modifications, public domain as well, by Antti Haapala, 11/10/17
   - Switched to getc on 5/23/19 */
// if typedef doesn't exist (msvc, blah)
typedef intptr_t ssize_t;

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

//https://stackoverflow.com/questions/17258029/c-setenv-undefined-identifier-in-visual-studio
int setenv(const char *name, const char *value, int overwrite) {
    int errcode = 0;
    if(!overwrite) {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, NULL, 0, name);
        if(errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}
