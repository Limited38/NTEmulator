#include "TConfig.h"
#include "stdafx.h"

TConfig Config;
//------------------------------------------------------------

bool TConfig::LoadConfigurationFiles()
{
	TCHAR Directory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Directory);

	std::string path = Directory;
	path += "\\configs\\server.ini";

	std::cout << "Called TConfig::LoadConfigurationFiles" << std::endl;

	//Ini File auslesen
	this->db_host = this->GetIniString("DATABASE", "HOST", path);
	this->db_user = this->GetIniString("DATABASE", "USER", path);
	this->db_password = this->GetIniString("DATABASE", "PASSWORD", path);
	this->db_schema = this->GetIniString("DATABASE", "SCHEMA", path);

	this->server_host = this->GetIniString("SERVER", "HOST", path);
	this->loginserver_port = this->GetIniValue("SERVER", "LOGIN_PORT", path);
	this->gameserver_port = this->GetIniValue("SERVER", "GAME_PORT", path);

	//Prüfen ob alles erfolgreich ausgelesen
	if(this->db_host.empty() || this->db_user.empty() ||
		this->db_password.empty() || this->db_schema.empty() || this->server_host.empty() ||
		this->loginserver_port == 0 || this->gameserver_port == 0)
	{
		std::cout << "Error: Config.LoadConfigurationFiles() failed" << std::endl;
		std::cout << "Path: " << path.c_str() << std::endl;
		std::cout << "Please press Enter to exit..." << std::endl;
		return 1;
	}

	std::cout << "TConfig::LoadConfigurationFiles successfully" << std::endl;

	return 0;
}

//------------------------------------------------------------

std::string TConfig::GetIniString(std::string section, std::string key, std::string path)
{
	std::vector<char> buffer(255);
	GetPrivateProfileString(section.c_str(), key.c_str(), NULL, &buffer.front(), buffer.size(), path.c_str());
	return &buffer.front();
}

//------------------------------------------------------------

int TConfig::GetIniValue(std::string section, std::string key, std::string path)
{
	return GetPrivateProfileInt(section.c_str(), key.c_str(), NULL, path.c_str());
}

//------------------------------------------------------------
