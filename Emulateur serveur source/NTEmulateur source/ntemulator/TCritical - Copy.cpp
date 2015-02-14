#include "stdafx.h"
#include "TCritical.h"

//------------------------------------------------------------

TCritical::TCritical(void)
{
	//Critical Section erstellen
	InitializeCriticalSection(&crit); 
}

//------------------------------------------------------------

TCritical::~TCritical(void)
{
	//Critical Section löschen
	DeleteCriticalSection(&crit);
}

//------------------------------------------------------------

void TCritical::Enter()
{
	//Critical Section betreten
	EnterCriticalSection(&crit); 
}

//------------------------------------------------------------

void TCritical::Leave(void)
{
	//Critical Section verlassen
	LeaveCriticalSection(&crit); 
}

//------------------------------------------------------------