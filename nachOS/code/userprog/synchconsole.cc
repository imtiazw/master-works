#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}
void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch);
	writeDone->P();
}
char SynchConsole::SynchGetChar()
{
	readAvail->P ();        // wait for character to arrive
	return console->GetChar();
}
void SynchConsole::SynchPutString(const char s[])
{
	int i=0;
	while(s[i]!='\0')
	{
		this->SynchPutChar(s[i]);
		i++;
	}
	//this->SynchPutChar('\0');
}
void SynchConsole::SynchGetString(char *s, int n)
{
	int i;
	i=0;
	while(s[i]!='\0' && i<=n-1 && s[i] != EOF)
	{
		readAvail->P();
		s[i] = console->GetChar();
		i++;
	}
}

void SynchConsole::SynchPutInt(int n)
{
	buffer = new char[MAX_BUFFER_SIZE];
	snprintf(buffer, MAX_BUFFER_SIZE, "%d", n);
	
	for(int i=0; i<MAX_BUFFER_SIZE-1 && buffer[i]!='\0'; i++){		//"MAX_BUFFER_SIZE - 1" for the
		this->SynchPutChar(buffer[i]);								//reason of last character to be
	}																//'\0'
	delete []buffer;			//to destry the buffer after it has been written
}

void SynchConsole::SynchGetInt(int *n)
{
	buffer = new char[MAX_BUFFER_SIZE];
	int i;
	char input;
	for(i=0; i<MAX_BUFFER_SIZE-1; i++)
	{
		input = this->SynchGetChar();
		if(input != '\n')
			buffer[i] = input;
		else
			break;
	}
	buffer[i] = '\0';
	sscanf(buffer, "%d", n);
	delete []buffer;
}
#endif // CHANGE
