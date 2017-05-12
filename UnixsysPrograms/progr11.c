/*
Write a C program to implement the syntax-directed definition of "if E then S1" and
"if E then S1 else S2"
*/
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
	printf("Enter the statement : \n");
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
/*
Output :
cc progr11.c -o progr11
./progr11
Format of if statement
Eg:
if (a<b) then (s,a)
if (a<b) then (s,a) else (s,b)
Enter the statement :
if A then S1 else S2

   if A GOTO 100
        GOTO 200
100:    S1
        GOTO:300
200:    S2
300:
if (a<b) then (s,a) else (s,b)
*/