#pragma once

#ifndef _H_CRITICAL
#define _H_CRITICAL

class TCritical
{
public:
	TCritical(void); //Konstruktor
	~TCritical(void); //Destruktor
	void Enter(void); //Critical Section verlassen
	void Leave(void); //Critical Section betreten
private:
	CRITICAL_SECTION crit;
};

#endif _H_CRITICAL