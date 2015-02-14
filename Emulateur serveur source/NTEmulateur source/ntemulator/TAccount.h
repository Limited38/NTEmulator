#pragma once

#ifndef _H_ACCOUNT
#define _H_ACCOUNT

//Typedefinitionen

class TSocket;
class TAccount;
extern TAccount Account;

struct structAccount
{
	bool isIngame;
	bool Checked;
	int accountid;
	std::string user;
	std::string password;
	int session;
};
//-------------------

class TAccount
{
public:
	bool Check(TSocket *Client); //Account überprüfen
};

#endif _H_ACCOUNT