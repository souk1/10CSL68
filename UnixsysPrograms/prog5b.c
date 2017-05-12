/*
Write s C/C++ program to emulate the unix ln command
*/
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
/*
Output :
touch f1
cc prog5b.c -o prog5b
./prog5b f1 f2
Hard link created
cat f2		(File exists if nothing is returned)
./prog5b -s f1 f3
Symbolic link created
cat f3		(File exists if nothing is returned)
*/