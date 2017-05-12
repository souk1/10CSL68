/*
Write a C/C++ program that outputs the content of its environment list
*/
#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[],char *envp[])
{
	int i=0;
	while(envp[i]!='\0')
	{
		printf("%s\n\n",envp[i]);
		i++;
	}
	return 0;
}
/*
Output : 
cc prog5a.c -o prog5a
./prog5a
XDG_VTNR=8
VSCODE_CLI=1
SSH_AGENT_PID=2203
XDG_SESSION_ID=c1
.
.
.
*/