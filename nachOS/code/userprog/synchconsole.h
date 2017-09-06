#ifdef CHANGED
#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "../threads/synch.h"

#define MAX_BUFFER_SIZE 10


class SynchConsole {
	public:
		SynchConsole(char *readFile, char *writeFile);
		// initialize the hardware console device
		~SynchConsole();                    // clean up console emulation
		void SynchPutChar(const char ch);   // Unix putchar(3S)
		char SynchGetChar();                // Unix getchar(3S)
		//char SynchGetChar(int * x);
		void SynchPutString(const char *s); // Unix puts(3S)
		void SynchGetString(char *s, int n);
		void SynchPutInt(int n);
		void SynchGetInt(int *n);
	private:
		Console *console;
		char character;
		char *buffer;
		Semaphore *bufferSemaphore;
};

#endif // SYNCHCONSOLE_H
#endif // CHANGED
