#pragma once

#ifndef _H_GENERAL
#define _H_GENERAL

//Typedef
class TGeneral;
extern TGeneral General;

class TGeneral
{
private:
	bool timeGenerated; //Prüfen ob srand bereits verwendet
	std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems); //Split Wrapper
public:
	TGeneral(); //Konstruktor
	~TGeneral(); //Destruktor

	int s_atoi(std::string str); //String in Integer konvertieren
	int CreateRandomNumber(int min, int max); //Zufallszahl zurückgeben
	std::string GetDateTime(); //Zeit und Datum zurückgeben
	std::vector<std::string> split(const std::string &s, char delim); //String split Funktion
	std::string GetExePath(); //Datei Pfad zurückgeben
	long GetSeconds(); //Sekunden seit 1935? zurückgeben
};

#endif _H_GENERAL
