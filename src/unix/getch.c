// Source: https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
#include <termios.h>
#include <stdio.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void _initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  current.c_lflag &= ~ISIG; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void _resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char _getch_(int echo) 
{
  char ch;
  _initTermios(echo);
  ch = getchar();
  _resetTermios();
  return ch;
}

/* Read 1 character without echo */
char _getch(void) 
{
  return _getch_(0);
}

/* Read 1 character with echo */
char _getche(void) 
{
  return _getch_(1);
}

/* Let's test it out */
//int main(void) {
//  char c;
//  printf("(getche example) please type a letter: ");
//  c = getche();
//  printf("\nYou typed: %c\n", c);
//  printf("(getch example) please type a letter...");
//  c = getch();
//  printf("\nYou typed: %c\n", c);
//  return 0;
//}
