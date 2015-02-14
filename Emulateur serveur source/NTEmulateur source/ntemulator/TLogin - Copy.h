#pragma once

#ifndef _H_LOGIN
#define _H_LOGIN

//Typedefinition

class TGeneral;

//--------------------

class TLogin
{
private:
	TGeneral *General;

	void QueryAccount(TSocket *Client, TDatabase *currentConnection, std::string user, std::string password); //Account Datenbank abfrage
	void QueryServerlist(TSocket *Client, TDatabase *currentConnection, int accountid); //Server Datenbank Update

public:
	TLogin(); //Konstruktor
	~TLogin(); //Dekonstruktor

	void VerifyAccount(TSocket *Client, std::string user, std::string password); //Account überprüfen
};

#endif _H_LOGIN