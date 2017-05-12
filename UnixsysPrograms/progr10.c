/*
Write a C/C++ program to set up a real-time clock interval timer using
the alarm API
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void callme(int signo)
{
	printf("Hello!\n");
}

int main()
{
	struct sigaction action;
	action.sa_handler=(void(*)(int))callme;
	sigaction(SIGALRM,&action,0);
	alarm(2);
	sleep(2);
	return 0;
}
/*
Output :
cc progr10.c -o progr10
./progr10
Hello!
*/