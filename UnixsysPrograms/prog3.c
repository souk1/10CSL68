/*
Consider the last 100 bytes as a region. Write a C/C++ program to check whether the
region is locked or not. If the region is locked, print pid of the process which has
locked. If the region is not locked, lock the region with an exclusive lock, read 
the last 50 bytes and unlock the region.
*/
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
/*
Output :
touch a
chmod 777 a
cc prog3.c -o prog3
./prog3

Not locked
File is locked exclusively
PID of the process = 0
Contents of the locked file :
11111111110000000000111111111100000000001111111111
File unlocked
*/

/*
Contents of a :
00000000001111111111000000000011111111110000000000
11111111110000000000111111111100000000001111111111
*/