#pragma once

#ifndef _H_DATABASE
#define _H_DATABASE

//Typedefinitionen
class TDatabase;
extern TDatabase Database;
//-----------------------------------

class TDatabase
{
private:
	sql::Driver *driver;
public:
	sql::Connection *connection;
	sql::ResultSet *result;
	sql::PreparedStatement *stmt;
	bool inUse;

	bool Initialize(std::string host, std::string user, std::string password, std::string schema); //Datenbank initialisieren
};

#endif _H_DATABASE

