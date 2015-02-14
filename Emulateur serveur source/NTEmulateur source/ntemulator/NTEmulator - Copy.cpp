// NTEmulator.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTitle("NTEmulator");
	std::cout << "Preparing to run the Server..." << std::endl;

	float calctime = clock();

	//Server Configs laden
	bool errors = Config.LoadConfigurationFiles();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//Items laden
	errors = Config.ReadItemList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//Gegner Typen laden

	errors = Config.ReadEnemyList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//NPCs Shop liste laden

	errors = Config.ReadShopList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//NPCs laden

	errors = Config.ReadNpcList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//Skills laden

	errors = Config.ReadSkillList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//Skills laden

	errors = Config.ReadEnemySkillList();

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//Server Maps laden
	errors = Config.LoadMapFiles();

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
	std::cout << "Rates: " << Config.Rate_XP << "/" << Config.Rate_JXP << "/" << Config.Rate_Drop << "/" << Config.Rate_Gold << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	//Server Datenbank Pool erstellen
	errors = Database.Initialize(Config.db_host, Config.db_user, Config.db_password, Config.db_schema);

	if(errors)
	{
		std::getchar();
		return 1;
	}

	//GameServer initialisieren
	errors = GameServer.Initialize(Config.server_host, Config.gameserver_port);

	if(errors)
	{
		std::getchar();
		return 1;
	}

	std::cout << "Creating Enemies Thread..." << std::endl;
	errors = CreateThread(NULL, 0, TEnemies::EnemyThread, (LPVOID)0, 0, 0 );

	if(errors == NULL)
	{
		std::cout << "Error while creating thread" << std::endl;
		std::getchar();
		return 1;
	}else
	{
		std::cout << "TEnemies::EnemyThread() created" << std::endl;
	}

	std::cout << "Creating Players Thread..." << std::endl;
	errors = CreateThread(NULL, 0, TCharacterInfo::PlayerThread, (LPVOID)0, 0, 0 );

	if(errors == NULL)
	{
		std::cout << "Error while creating thread" << std::endl;
		std::getchar();
		return 1;
	}else
	{
		std::cout << "TCharacterInfo::PlayerThread() created" << std::endl;
	}

	std::cout << "Creating Items Thread..." << std::endl;
	errors = CreateThread(NULL, 0, TItems::ItemsThread, (LPVOID)0, 0, 0 );

	if(errors == NULL)
	{
		std::cout << "Error while creating thread" << std::endl;
		std::getchar();
		return 1;
	}else
	{
		std::cout << "TItems::ItemsThread() created" << std::endl;
	}

	std::cout << "Server started without any problems" << std::endl;
	std::cout << "Time needed: " << (float)((clock() - calctime) / CLOCKS_PER_SEC) << std::endl;
	std::cout << "Entering TServer::AcceptConnection loop" << std::endl;

	do 
	{
		GameServer.AcceptConnections();
	} while (1);

	return 0;
}

