# Unix System Programming and Compiler Design Lab
___

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
Output :</br>
$ cc prog1.c -o prog1</br>
$ ./prog1</br>
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
Output :</br>
cc prog2.c -o prog2</br>
./prog2</br>
System supports job control</br>
System supports saved UIDs and GIDs</br>
System restricts change of file ownership</br>
System supports truncation</br>
System supports no disabling character</br>
___

