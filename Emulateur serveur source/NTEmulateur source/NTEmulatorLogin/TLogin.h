#pragma once

#ifndef _H_LOGIN
#define _H_LOGIN

//Typedefinition

class TLogin;
extern TLogin Login;

//--------------------

class TLogin
{
private:
	bool QueryAccount(TSocket *Client, std::string user, std::string password); //Account Datenbank abfrage
	void QueryServerlist(TSocket *Client, int accountid); //Server Datenbank Update

public:
	TLogin(); //Konstruktor
	~TLogin(); //Dekonstruktor

	void VerifyAccount(TSocket *Client, std::string user, std::string password); //Account überprüfen
};

#endif _H_LOGIN