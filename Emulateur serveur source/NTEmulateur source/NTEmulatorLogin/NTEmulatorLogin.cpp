// NTEmulatorLogin.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTitle("NTEmulator");
	std::cout << "Preparing to run the Server..." << std::endl;

	//Server Configs laden
	bool errors = Config.LoadConfigurationFiles();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Database Host: " << Config.db_host.c_str() << std::endl;
	std::cout << "Database User: " << Config.db_user.c_str() << std::endl;
	std::cout << "Database Password: " << Config.db_password.c_str() << std::endl;
	std::cout << "Database Schema: " << Config.db_schema.c_str() << std::endl << std::endl;
	std::cout << "Server Host: " << Config.server_host.c_str() << std::endl;
	std::cout << "LoginServer Port: " << Config.loginserver_port << std::endl;
	std::cout << "GameServer Port: " << Config.gameserver_port << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	//Server Datenbank Pool erstellen
	errors = Database.Initialize(Config.db_host, Config.db_user, Config.db_password, Config.db_schema);

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//GameServer initialisieren
	errors = LoginServer.Initialize(Config.server_host, Config.loginserver_port);

	if(errors)
	{
		std::getchar();
		return 1;
	}

	std::cout << "Server started without any problems" << std::endl;
	std::cout << "Entering TServer::AcceptConnection loop" << std::endl << std::endl;;

	do 
	{
		LoginServer.AcceptConnections();
	} while (1);

	return 0;
}

