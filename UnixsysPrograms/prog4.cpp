/*
Write a C/C++ program which demonstrates interprocess communication between
a reader process and a writer process. Use mkfifo, open, read, write and close
APIs in your program
*/
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
/*
Output :
g++ prog4.cpp -o prog4
./prog4 fifo "Hello!" &
[1] 5672
./prog4 fifo
Hello!
[1]+ Done 
*/