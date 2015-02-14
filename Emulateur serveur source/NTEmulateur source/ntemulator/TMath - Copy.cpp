#include "stdafx.h"
#include "TMath.h"

//Typedefinitionenn

TMath Math;

//------------------------------------------------------------

TMath::TMath(void)
{
}

//------------------------------------------------------------

TMath::~TMath(void)
{
}

//------------------------------------------------------------

int TMath::Distance(int x1, int x2, int y1, int y2)
{
	//Wurzel anhand der Positionen ziehen
	return(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

//------------------------------------------------------------
