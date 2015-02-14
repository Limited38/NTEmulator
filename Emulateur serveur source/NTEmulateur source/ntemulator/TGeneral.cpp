#include "stdafx.h"
#include "TGeneral.h"

TGeneral General;
//------------------------------------------------------------

TGeneral::TGeneral()
{
	this->timeGenerated = 0;
}

//------------------------------------------------------------

TGeneral::~TGeneral()
{
}

//------------------------------------------------------------

int TGeneral::CreateRandomNumber(int min, int max)
{
	//Wenn Seed noch nicht generiert, generieren
	if(!this->timeGenerated)
	{
		std::srand(time(0));
		this->timeGenerated = true;
	}

	//Random Zahl zurückgeben
	return rand() % (max - min) + min;
}

//------------------------------------------------------------

std::string TGeneral::GetDateTime()
{
	//Aktuelle Uhrzeit und Zeit auslesen und zurückgeben
	time_t timestamp = time(&timestamp);
	std::vector<char> currenttime(30);
	ctime_s(&currenttime.front(), 30, &timestamp);
	return &currenttime.front();
}

//------------------------------------------------------------

std::vector<std::string> TGeneral::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;

	//Tokens splitten
	while(getline(ss, item, delim))
	{ 
		//Splitted Strings in den Vector pushen
		elems.push_back(item);
	}

	//Vector returnen
	return elems;
}

//------------------------------------------------------------

std::vector<std::string> TGeneral::split(const std::string &s, char delim)
{
	//Split Wrapper aufrufen und returnen
	std::vector<std::string> elems;
	return split(s, delim, elems);
}

//------------------------------------------------------------

std::string TGeneral::GetExePath()
{
	//Pfad auslesen und zurückgeben
	TCHAR Directory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Directory);
	return Directory;
}

//------------------------------------------------------------

long TGeneral::GetSeconds()
{
	//Sekunden seit 1935? zurückgeben
	return time(NULL);
}

//------------------------------------------------------------

int TGeneral::s_atoi( std::string str )
{
	int value;
	std::stringstream ss;
	
	//String nach INT konvertieren und zurückgeben
	ss << str; 
	ss >> value; 
	return value; 
}

//------------------------------------------------------------

float TGeneral::s_ToFloat( std::string str )
{
	float value;
	std::stringstream ss;

	//String nach FLOAT konvertieren und zurückgeben
	ss << str; 
	ss >> value; 
	return value; 
}

//------------------------------------------------------------