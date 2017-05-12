/*
Write a C/C++ POSIX compliant program to check the following limits:
1. Number of clock ticks
2. Max number of child processes
3. Max path length
4. Max number of characters in a file name
5. Max number of open files/processes
*/

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
Many system expects argument of type long int (%ld) instead of int (%d) <--Can use int type also 
*/

/*
Output :
cc prog1.c -o prog1
./prog1
Runtime values
The max number of clock ticks : 100
The max runtime child processes : 15522
The max runtime path length : 4096
The max characters in a file name : 255
The max number of opened files : 1024
*/