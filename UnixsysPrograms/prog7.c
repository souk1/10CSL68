/*
Write a C/C++ program that creates a zombie and then calls system to
execute the ps command to verify that the process is zombie
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#define PS "ps -o pid,ppid,state,tty,command"

int main()
{
	pid_t pid;
	if((pid=fork())<0)
		printf("Fork error\n");
	else if(pid==0)
		exit(0);
	sleep(2);
	system(PS);
	exit(0);
}
/*
Output :
cc prog7.c -o prog7
./prog7
  PID  PPID S TT       COMMAND
 3260  3257 S pts/1    /bin/bash
 4789  3260 S pts/1    ./prog7
 4790  4789 Z pts/1    [prog7] <defunct>
 4791  4789 S pts/1    sh -c ps -o pid,ppid,state,tty,command
 4792  4791 R pts/1    ps -o pid,ppid,state,tty,command
*/