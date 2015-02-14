#include "stdafx.h"
#include "TDatabase.h"

//Typedefinitionen

TDatabase Database;

//------------------------------------------------------------

bool TDatabase::Initialize(std::string host, std::string user, std::string password, std::string schema)
{
	try
	{
		//Instanz erstellen
		this->driver = get_driver_instance(); 

		//Verbindung zur Datenbank herstellen
		this->connection = this->driver->connect(host, user, password);

		//Table setzen
		this->connection->setSchema(schema);

	} catch(sql::SQLException &e)
	{
		std::cout << "Error: TDatabase::Initialize() failed" << std::endl;
		return 1;
	}

	return 0;
}

//------------------------------------------------------------
