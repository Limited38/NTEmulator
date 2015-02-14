#pragma once


#ifndef _H_MATH
#define _H_MATH

//Typedefinitionen
class TMath;
extern TMath Math;
//-----------------------

class TMath
{
private:

public:
	TMath(void); //Konstruktor
	~TMath(void); //Destruktor

	int Distance(int x1, int x2, int y1, int y2); //Distanz ausrechnen
};

#endif _H_MATH