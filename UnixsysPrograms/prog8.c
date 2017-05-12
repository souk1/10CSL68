/*
Write a C/C++ program to avoid zombie process by forking twice
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void)
{
	pid_t pid;
	if((pid=fork())<0)
		printf("Fork error\n");
	else if(pid==0)
	{
		if((pid=fork())<0)
			printf("Fork error\n");
		else if(pid>0)
			exit(0);
		system("ps -o pid,ppid,state,tty,command");
		sleep(2);
		printf("\nSecond child, parent pid = %d\n",getpid());
		exit(0);
	}
	if(waitpid(pid,NULL,0)!=pid)
		printf("Waitpid error\n");
	exit(0);
}
/*
Output :
cc prog8.c -o prog8
./prog8
  PID  PPID S TT       COMMAND
 3260  3257 S pts/1    /bin/bash
 5332     1 S pts/1    ./prog8
 5333  5332 S pts/1    sh -c ps -o pid,ppid,state,tty,command
 5334  5333 R pts/1    ps -o pid,ppid,state,tty,command

Second child, parent pid = 5332
*/