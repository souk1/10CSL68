/*
Write a C/C++ program to illustrate the race condition
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

static void charatatime(char *);

int main()
{
	pid_t pid;
	if((pid=fork())<0)
		printf("Fork error\n");
	else if(pid==0)
		charatatime("Output from child\n");
	else
		charatatime("Output from parent\n");
	exit(0);
}

static void charatatime(char * str)
{
	char *ptr;
	int c;
	setbuf(stdout,NULL);
	for(ptr=str;(c=*ptr++)!=0;)
		putc(c,stdout);
}
/*
Output :
cc prog6.c -o prog6
./prog6
Output frOoumt ppuatr efnrto
m child				(if output is not random exec ./prog6 many times)
*/