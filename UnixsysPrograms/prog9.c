/*
Write a C/C++ program to implement the system function
*/
#include<sys/wait.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int system1(const char * cmdstr)
{
	pid_t pid;
	int status;
	if(cmdstr==NULL)
		return(1);
	if((pid=fork())<0)
		status=-1;
	else if(pid==0){
		execl("/bin/sh","sh","-c",cmdstr,(char *)0);
		_exit(127);
	}
	else
		while(waitpid(pid,&status,0)<0)
			if(errno!=EINTR){
				status=-1;
				break;
			}
}
int main(int argc, char * argv[])
{
	if(system1(argv[1])<0)
		printf("sytem() error\n");
	exit(0);
}
/*
Output :
cc prog9.c -o prog9
./prog9 date
Fri May 12 08:10:11 IST 2017
*/