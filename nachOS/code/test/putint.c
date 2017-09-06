#include "syscall.h"

void print(int n, int limit)
{
	int i;
	for(i=0; i<limit; i++)
	{
		SynchPutInt(n+i);
		PutChar('\n');
	}
}

int
main()
{
	print('c', 5);
	Halt();
}