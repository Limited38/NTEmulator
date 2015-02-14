#pragma once

#ifndef _H_CONFIG
#define _H_CONFIG

class TConfig;
extern TConfig Config;

class TConfig
{
private:
	std::string GetIniString(std::string section, std::string key, std::string path); //String auslesen von ini Datei
	int GetIniValue(std::string section, std::string key, std::string path); //Integer auslesen von ini Datei
public:

	//Variablen Server einstellungen
	std::string db_host;
	std::string db_user;
	std::string db_password;
	std::string db_schema;

	std::string server_host;
	u_short loginserver_port;
	u_short gameserver_port;
	//------------------------------------


	bool LoadConfigurationFiles(); //ini Einstellung auslesen
};

#endif _H_CONFIG
