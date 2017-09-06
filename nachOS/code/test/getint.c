#include "syscall.h"
int main()
{	
	int n;
	SynchGetInt(&n);
	SynchPutInt(n);
	PutChar('\n');
	Halt();
	return 0;
}

