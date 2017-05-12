/*
Write a C/C++ POSIX compliant program that prints the POSIX defined configuration 
options supported on any given system using feature test macros
*/
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
/*
Output :
cc prog2.c -o prog2
./prog2
System supports job control
System supports saved UIDs and GIDs
System restricts change of file ownership
System supports truncation
System supports no disabling character
*/