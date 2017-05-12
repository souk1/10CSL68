# Unix System Programming and Compiler Design Lab

<img src="https://octodex.github.com/images/stormtroopocat.png" alt="Stormtroopocat" width="400" height="400" align="right" title="May the CODE be with you">

## Contents
- [Check POSIX runtime limits](#program1)
- [POSIX feature test macros](#program2)
- [Lock & Unlock regions in file](#program3)
- [Inter-process communication](#program4)
- [Environment list](#program5a)
- [Ln command](#program5b)
- [Race condition](#program6)
- [Creating zombie process](#program7)
- [Avoiding zombie process](#program8)
- [System function](#program9)
- [Alarm API](#program10)
- [Syntax-directed definition](#program11)
- [Parse tree production](#program12)
___

## 1. Check POSIX runtime limits <a id="program1"></a>
Write a C/C++ POSIX compliant program to check the following limits:
1. Number of clock ticks
2. Max number of child processes
3. Max path length
4. Max number of characters in a file name
5. Max number of open files/processes
```c
#include<stdio.h>
#include<unistd.h>
#include<limits.h>
//#define _POSIX_SOURCE
//#define _POSIX_C_SOURCE 199309L

int main()
{
    printf("Runtime values\n");
    printf("The max number of clock ticks : %ld\n",sysconf(_SC_CLK_TCK));
    printf("The max runtime child processes : %ld\n",sysconf(_SC_CHILD_MAX));
    printf("The max runtime path length : %ld\n",pathconf("prog1.c",_PC_PATH_MAX));
    printf("The max characters in a file name : %ld\n",pathconf("prog1.c",_PC_NAME_MAX));
    printf("The max number of opened files : %ld\n",sysconf(_SC_OPEN_MAX));
    return 0;
}
/*
Many system expects argument of type long int (%ld) instead of int (%d) <-- Can use int type also 
*/
```
**Output** :</br>
**$** cc prog1.c -o prog1</br>
**$** ./prog1</br>
Runtime values</br>
The max number of clock ticks : 100</br>
The max runtime child processes : 15522</br>
The max runtime path length : 4096</br>
The max characters in a file name : 255</br>
The max number of opened files : 1024</br>

___

## 2. POSIX feature test macros <a id="program2"></a>
Write a C/C++ POSIX compliant program that prints the POSIX defined configuration</br>
options supported on any given system using feature test macros
```c
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<unistd.h>

int main()
{
	#ifdef _POSIX_JOB_CONTROL
		printf("\nSystem supports job control\n");
	#else
		printf("No system support\n");
	#endif
	#ifdef _POSIX_SAVED_IDS
		printf("System supports saved UIDs and GIDs\n");
	#else
		printf("No system support\n");
	#endif
	#ifdef _POSIX_CHOWN_RESTRICTED
		printf("System restricts change of file ownership\n");
	#else
		printf("No system support\n");
	#endif
	#ifdef _POSIX_NO_TRUNC
		printf("System supports truncation\n");
	#else
		printf("No system support\n");
	#endif
	#ifdef _POSIX_VDISABLE
		printf("System supports no disabling character\n\n");
	#else
		printf("No system support\n");
	#endif
	return 0;
}
```
**Output** :</br>
**$** cc prog2.c -o prog2</br>
**$** ./prog2</br>
System supports job control</br>
System supports saved UIDs and GIDs</br>
System restricts change of file ownership</br>
System supports truncation</br>
System supports no disabling character</br>
___

## 3. Lock & Unlock regions in file <a id="program3"></a>
Consider the last 100 bytes as a region. Write a C/C++ program to check whether the</br>
region is locked or not. If the region is locked, print pid of the process which has</br>
locked. If the region is not locked, lock the region with an exclusive lock, read</br>
the last 50 bytes and unlock the region.</br>
```c
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>

int main(int argc,char * argv[])
{
	struct flock fc;
	int fd;
	char buf[256];
	fd=open(argv[1],O_RDWR);
	if(fd==-1)
	{
		printf("Error\n");
		return 1;
	}
	lseek(fd,-100,SEEK_CUR);
	fc.l_whence=SEEK_CUR;
	fc.l_start=0;
	fc.l_len=100;
	if(fcntl(fd,F_GETLK,&fc)==0)
	{
		printf("File locked\n");
		printf("PID of the process = %d\n",fc.l_pid);
		return 0;
	}
	else{
		printf("Not locked\n");
		fc.l_type=F_WRLCK;
		fc.l_whence=SEEK_END;
		fc.l_start=0;
		fc.l_len=100;
		if(fcntl(fd,F_SETLK,&fc)==-1)
		{
			printf("Error in locking file\n");
			return 1;
		}
		else{
			printf("File is locked exclusively\n");
			printf("PID of the process = %d\n",fc.l_pid);
			lseek(fd,-50,SEEK_END);
			printf("Contents of the locked file : \n");
			buf[50]='\0';
			if(read(fd,buf,sizeof(buf)))
				printf("%s",buf);
		}
	}
	fc.l_type=F_UNLCK;
	fc.l_whence=SEEK_END;
	fc.l_start=0;
	fc.l_len=100;
	if(fcntl(fd,F_SETLKW,&fc)==-1)
	{
		printf("Error\n");
		return 1;
	}
	else{
		printf("\nFile unlocked\n");
		return 0;
	}
}
```
**Output** :</br>
**$** touch a</br>
**$** chmod 777 a</br>
**$** cc prog3.c -o prog3</br>
**$** ./prog3</br>
</br>
Not locked</br>
File is locked exclusively</br>
PID of the process = 0</br>
Contents of the locked file :</br>
11111111110000000000111111111100000000001111111111</br>
File unlocked</br>
</br>
**Contents of a** :</br>
00000000001111111111000000000011111111110000000000</br>
11111111110000000000111111111100000000001111111111</br>
___

## 4. Inter-process communication <a id="program4"></a>
Write a C/C++ program which demonstrates interprocess communication between</br>
a reader process and a writer process. Use mkfifo, open, read, write and close</br>
APIs in your program</br>
```c++
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=2 && argc!=3)
	{
		cout<<"Usage : "<<argv[0]<<"<file>[<argv>]"<<endl;
		return 0;
	}
	int fd;
	char buf[256];
	(void)mkfifo(argv[1],S_IFIFO|S_IRWXU|S_IRWXG|S_IRWXO);
	if(argc==2)
	{
		fd=open(argv[1],O_RDONLY);  //O_NONBLOCK is not needed
		while(read(fd,buf,sizeof(buf))>0)
			cout<<buf<<endl;
	}
	else{
		fd=open(argv[1],O_WRONLY);
		write(fd,argv[2],strlen(argv[2]));
	}
	close(fd);
	return 0;
}
```
**Output** :</br>
**$** g++ prog4.cpp -o prog4</br>
**$** ./prog4 fifo "Hello!" &</br>
[1] 5672</br>
**$** ./prog4 fifo</br>
Hello!</br>
[1]+ Done</br>
___

## 5a. Environment list <a id="program5a"></a>
Write a C/C++ program that outputs the content of its environment list</br>
```c
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
```
**Output** : </br>
**$** cc prog5a.c -o prog5a</br>
**$** ./prog5a</br>
XDG_VTNR=8</br>
VSCODE_CLI=1</br>
SSH_AGENT_PID=2203</br>
XDG_SESSION_ID=c1</br>
.</br>
.</br>
.</br>
___

## 5b. Ln command <a id="program5b"></a>
Write s C/C++ program to emulate the unix ln command</br>
```c
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char * argv[])
{
	if(argc==3)
	{
		if(link(argv[1],argv[2])==-1)
			printf("Error\n");
		else
			printf("Hard link created\n");
	}
	else if(argc==4 && strcmp(argv[1],"s"))  //"-s" can be used instead of "s"
	{
		if(symlink(argv[2],argv[3])==-1)
			printf("Error\n");
		else
			printf("Symbolic link created\n");
	}
	else
		printf("Hey there, missing some arguments\n");
	return 0;
}
```
**Output** :</br>
**$** touch f1</br>
**$** cc prog5b.c -o prog5b</br>
**$** ./prog5b f1 f2</br>
Hard link created</br>
**$** cat f2		(File exists if nothing is returned)</br>
**$** ./prog5b -s f1 f3</br>
Symbolic link created</br>
**$** cat f3		(File exists if nothing is returned)</br>
___

## 6. Race condition <a id="program6"></a>
Write a C/C++ program to illustrate the race condition</br>
```c
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
```
**Output** :</br>
**$** cc prog6.c -o prog6</br>
**$** ./prog6</br>
<pre>Output frOoumt ppuatr efnrto
m child				(if output is not random exec ./prog6 many times)</pre>
___

## 7. Creating zombie process <a id="program7"></a>
Write a C/C++ program that creates a zombie and then calls system to</br>
execute the ps command to verify that the process is zombie</br>
```c
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
```
**Output** :</br>
**$** cc prog7.c -o prog7</br>
**$** ./prog7</br>
<pre>  PID  PPID S TT       COMMAND
 3260  3257 S pts/1    /bin/bash
 4789  3260 S pts/1    ./prog7
 4790  4789 Z pts/1    [prog7] <defunct>
 4791  4789 S pts/1    sh -c ps -o pid,ppid,state,tty,command
 4792  4791 R pts/1    ps -o pid,ppid,state,tty,command</pre>
___

## 8. Avoiding zombie process <a id="program8"></a>
Write a C/C++ program to avoid zombie process by forking twice</br>
```c
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
```
**Output** :</br>
**$** cc prog8.c -o prog8</br>
**$** ./prog8</br>
<pre>  PID  PPID S TT       COMMAND
 3260  3257 S pts/1    /bin/bash
 5332     1 S pts/1    ./prog8
 5333  5332 S pts/1    sh -c ps -o pid,ppid,state,tty,command
 5334  5333 R pts/1    ps -o pid,ppid,state,tty,command</pre>
Second child, parent pid = 5332</br>
___

## 9. System function <a id="program9"></a>
Write a C/C++ program to implement the system function</br>
```c
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
```
**Output** :</br>
**$** cc prog9.c -o prog9</br>
**$** ./prog9 date</br>
Fri May 12 08:10:11 IST 2017</br>
___

## 10. Alarm API <a id="program10"></a>
Write a C/C++ program to set up a real-time clock interval timer using</br>
the alarm API</br>
```c
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
```
**Output** :</br>
**$** cc progr10.c -o progr10</br>
**$** ./progr10</br>
Hello!</br>
___

## 11. Syntax-directed definition <a id="program11"></a>
Write a C program to implement the syntax-directed definition of "if E then S1" and</br>
"if E then S1 else S2"</br>
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void gen(char[], char[], char[], int);

int main()
{
	int count=0,elseflag=0,i=0;
	char stmt[100];
	char tok[10][20];
	char *p;
	printf("Format of if statement\nEg:\n");
	printf("if (a<b) then (s,a)\n");
	printf("if (a<b) then (s,a) else (s,b)\n");
	printf("\nEnter the statement : \n");
	scanf("%[^\n]",stmt);
	p=strtok(stmt," ");
	while(p!=NULL)
	{
		strcpy(tok[i++],p);
		if(strcmp(p,"else")==0)
			elseflag=1;
		p=strtok(NULL," ");
	}
	if(strcmp(tok[2],"then")!=0)
	{
		printf("Invalid expression\n");
		return 0;
	}
	else if(elseflag && strcmp(tok[4],"else")!=0)
	{
		printf("Invalid expression\n");
		return 0;
	}
	else
		gen(tok[1],tok[3],tok[5],elseflag);
	return 0;
}
void gen(char c[],char s1[],char s2[],int elseflag)
{
	int L1=100,L2=200,L3=300;
	printf("\nif %s GOTO %d",c,L1);
	printf("\n\tGOTO %d",L2);
	printf("\n%d:\t%s",L1,s1);
	if(!elseflag)
		printf("\n%d:\n",L2);
	else{
		printf("\n\tGOTO:%d",L3);
		printf("\n%d:\t%s",L2,s2);
		printf("\n%d:\nif (a<b) then (s,a) else (s,b)\n",L3);
	}
}
```
**Output** :</br>
**$** cc progr11.c -o progr11</br>
**$** ./progr11</br>
*Format of if statement*</br>
*Eg:*</br>
if (a<b) then (s,a)</br>
if (a<b) then (s,a) else (s,b)</br></br>
Enter the statement :</br>
if A then S1 else S2</br>
<pre>   if A GOTO 100
        GOTO 200
100:    S1
        GOTO:300
200:    S2
300:</pre>
if (a<b) then (s,a) else (s,b)</br>
___

## 12. Parse tree production <a id="program12"></a>
Write a yacc program that accepts a regular expression as input and</br>
produce its parse tree as output</br>
```c
%{
	#include<stdio.h>
	#include<ctype.h>
	#include<stdlib.h>
	#include<string.h>
	#define MAX 100

	int getREindex(const char *);
	signed char production[MAX][MAX];
	int count=0,i,j;
	char temp[MAX+MAX],temp2[MAX+MAX];
%}
%token ALPHABET
%left '|'
%left '.'
%nonassoc '+''*'
%%
S:re	"\n"	{
			printf("This is right most derivation\n");
			for(i=count-1;i>=0;--i)
			{
				if(i==count-1){
					printf("\n re =>");
					strcpy(temp,production[i]);
					printf("%s",production[i]);
				}
				else{
					printf("\n  => ");
					j=getREindex(temp);
					temp[j]='\0';
					sprintf(temp2,"%s%s%s",temp,production[i],(temp+j+2));
					printf("%s",temp2);
					strcpy(temp,temp2);
				}
			}
			printf("\n");
			exit(0);
		}
re:ALPHABET	{	temp[0]=yylval;
			temp[1]='\0';
			strcpy(production[count++],temp);
		}
  |'('re')'	{	strcpy(production[count++],"(re)");	}
  |re'*'	{	strcpy(production[count++],"re*");	}
  |re'+'	{	strcpy(production[count++],"re+");	}
  |re'|'re	{	strcpy(production[count++],"re|re");	}
  |re'.'re	{	strcpy(production[count++],"re.re");	}
  ;
%%
int main(int argc, char * argv[])
{
	yyparse();
	return 0;
}
yylex()
{
	signed char ch=getchar();
	yylval=ch;
	if(isalpha(ch))
		return ALPHABET;
	return ch;
}
yyerror()
{
	fprintf(stderr,"Invalid regular expression\n");
	exit(1);
}
int getREindex(const char * str)
{
	int i=strlen(str)-1;
	for(;i>=0;--i)
		if(str[i]=='e' && str[i-1]=='r')
			return i-1;
}
```
**Output** :</br>
**$** yacc -d progr12.y</br>
**$** cc y.tab.c</br>
**$** ./a.out</br>
(a*|b+)*.(a+|b*)+</br>
This is right most derivation.
<pre>re	=>	re.re
	=>	re.re+
	=>	re.(re)+
	=>	re.(re|re)+
	=>	re.(re|re*)+
	=>	re.(re|b*)+
	=>	re.(re+|b*)+
	=>	re.(a+|b*)+
	=>	re*.(a+|b*)+
	=>	(re)*.(a+|b*)+
	=>	(re|re)*.(a+|b*)+
	=>	(re|re+)*.(a+|b*)+
	=>	(re|b+)*.(a+|b*)+
	=>	(re*|b+)*.(a+|b*)+
	=>	(a*|b+)*.(a+|b*)+</pre>
