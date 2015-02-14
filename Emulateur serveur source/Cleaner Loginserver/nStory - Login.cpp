#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// enable memory leak detector on debug modus
#ifdef _DEBUG
#include <vld.h>
#endif

#include <iostream>
#include <boost\thread.hpp>
#include <boost\algorithm\string.hpp>

#include "Config.h"
#include "GlobalConfig.h"
#include "Log.h"
#include "Database.h"
#include "Server.h"
#include "InputHandler.h"
#include "Tools.h"

// global declaration
boost::asio::io_service io_service;
std::unique_ptr<Nostale::Network::Server> server;

// typedefs
typedef std::unique_ptr<Nostale::Files::Config> p_config;
typedef std::unique_ptr<Nostale::Network::Database> p_database;

// console command functions
void cmdEnableMaintenance(const std::vector<std::string>& input);
void cmdDisableMaintenance(const std::vector<std::string>& input);
void cmdOutputHelp(const std::vector<std::string>& input);
void cmdServerExit(const std::vector<std::string>& input);
void cmdBlockConnection(const std::vector<std::string>& input);
void cmdUnblockConnection(const std::vector<std::string>& input);
void cmdShowBlockedConnections(const std::vector<std::string>& input);

void initConsoleInputHandler(InputHandler& consoleInputHandler);
void initDatabaseConnection(p_config& config);
void initGlobalConfigs(p_config& config);
void initServer(p_config& config);
void initConfig(p_config& config);
void initAll(p_config& config);

void cleanupServer(void);
void setServerConnectionFilter(void);

int main(void)
{
	p_config config;
	InputHandler consoleInputHandler;
	initConsoleInputHandler(consoleInputHandler);

	SetConsoleTitle("NSTORY - LOGIN");
	std::cout << "NSTORY PSERVER EMULATOR" << std::endl;
	std::cout << "This software was developed by Trollface- and is only for private usage!" << std::endl;
	std::cout << "Input /help for console commands." << std::endl << std::endl; 
	
	try
	{
		pantheios_be_file_setFilePath(panthstr("nstory_login.log"), PANTHEIOS_BE_FILE_F_TRUNCATE, 0, PANTHEIOS_BEID_ALL);
		panth::log_INFORMATIONAL(panthstr("[Server] Application started, configurating nstory server."));

		initAll(config);		
	} catch(const std::exception& e)
	{
		panth::log_EMERGENCY(panthstr("[Server] "), e.what());
	}

	{
	
	panth::log_WARNING(panthstr("[Server] Exiting."));
	std::cout << "Press enter to exit..." << std::endl;
	cleanupServer();
	return 0;
}

void cleanupServer(void)
{
	// cleanup when server running
	if(server)
	{
		server->disconnect();
		io_service.stop();
	}
}

void initAll(p_config& config)
{
	// initialize all
	initConfig(config);
	initGlobalConfigs(config);
	initDatabaseConnection(config);
	initServer(config);
}

void initConfig(p_config& config)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");

	const std::string configurationFile = "nstory_login.ini";

	BOOST_LOG(globalLog::get()) << "Reading configuration file '" << configurationFile << "'";
	std::cout << "Reading configuration file '" << configurationFile << "'" << std::endl;

	// initialize config file
	config = p_config(new Nostale::Files::Config(configurationFile));

	BOOST_LOG(globalLog::get()) << "Configuration file successfully read.";
	std::cout << "Configuration file successfully read." << std::endl;
}

void initServer(p_config& config)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");

	auto server_host = config->get<std::string>("Server.Host");
	auto server_port = config->get<signed short>("Server.Port");

	BOOST_LOG(globalLog::get()) << "Initializing server on endpoint('" << server_host << ":" << server_port << "')";
	std::cout << "Initializing server on endpoint('" << server_host << ":" << server_port << "')" << std::endl;
	
	// initialize server
	server = std::unique_ptr<Nostale::Network::Server>(new Nostale::Network::Server(io_service, server_host, server_port));
	setServerConnectionFilter();

	BOOST_LOG(globalLog::get()) << "Server initialized successfully.";
	BOOST_LOG(globalLog::get()) << "Waiting for incoming connections.";
	std::cout << "Server initialized successfully." << std::endl;
	std::cout << "Waiting for incoming connections..." << std::endl << std::endl;

	// begin server loop
	io_service.run();
}

void initGlobalConfigs(p_config& config)
{
	// set global configurations
	Nostale::GlobalConfig::DXClientHash = config->get<std::string>("ClientAuth.DXClientHash");
	Nostale::GlobalConfig::GLClientHash = config->get<std::string>("ClientAuth.GLClientHash");
	Nostale::GlobalConfig::ClientVersion = config->get<std::string>("ClientAuth.Version");
}

void initDatabaseConnection(p_config& config)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");

	auto db_host = config->get<std::string>("Database.Host");
	auto db_port = config->get<std::string>("Database.Port");
	auto db_user = config->get<std::string>("Database.User");
	auto db_password = config->get<std::string>("Database.Password");
	auto db_schema = config->get<std::string>("Database.Schema");
	auto db_pool_size = config->get<std::size_t>("Database.PoolSize");
	
	BOOST_LOG(globalLog::get()) << "Trying to connect to database on endpoint('" << db_host << ":" << db_port << "')" 
		<< " user: ('" << db_user << "') ...";
	std::cout << "Trying to connect to database on endpoint('" << db_host << ":" << db_port << "')" 
		<< " user: ('" << db_user << "') ..." << std::endl;

	// initialize database
	Nostale::Network::globalDatabase = p_database(new Nostale::Network::Database(
		db_host, db_port, db_user, db_password, db_schema, db_pool_size));

	BOOST_LOG(globalLog::get()) << "Successfully connected to database.";
	std::cout << "Successfully connected to database." << std::endl;
}

void initConsoleInputHandler(InputHandler& consoleInputHandler)
{
	// add console commands
	consoleInputHandler.add("/enable_maintenance", 1, &cmdEnableMaintenance);
	consoleInputHandler.add("/disable_maintenance", 1, &cmdDisableMaintenance);
	consoleInputHandler.add("/help", 1, &cmdOutputHelp);
	consoleInputHandler.add("/exit", 1, &cmdServerExit);
	consoleInputHandler.add("/block_connection", 2, &cmdBlockConnection);
	consoleInputHandler.add("/unblock_connection", 2, &cmdUnblockConnection);
	consoleInputHandler.add("/show_blocked_connections", 1, &cmdShowBlockedConnections);
	consoleInputHandler.start();
}

void cmdEnableMaintenance(const std::vector<std::string>& input)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");
	if(!Nostale::GlobalConfig::Maintenance)
	{
		// enable server maintenance		
		BOOST_LOG(globalLog::get()) << "Server maintenance was enabled.";
		std::cout << "Server maintenance was enabled." << std::endl;
		Nostale::GlobalConfig::Maintenance = true;
	} else
	{
		BOOST_LOG(globalLog::get()) << "Server maintenance already enabled.";
		std::cout << "Server maintenance already enabled." << std::endl;
	}
}

void cmdDisableMaintenance(const std::vector<std::string>& input)
{
	BOOST_LOG_SCOPED_THREAD_TAG("log", "nstory_login");
	if(Nostale::GlobalConfig::Maintenance)
	{
		// disable server maintenance
		BOOST_LOG(globalLog::get()) << "Server maintenance was disabled.";
		std::cout << "Server maintenance was disabled." << std::endl;
		Nostale::GlobalConfig::Maintenance = false;
	} else
	{
		BOOST_LOG(globalLog::get()) << "Server maintenance already disabled.";
		std::cout << "Server maintenance already disabled." << std::endl;
	}
}

void cmdOutputHelp(const std::vector<std::string>& input)
{
	std::cout << "Outputting help..." << std::endl;
	std::cout << "/enable_maintenance (enable server maintenance)" << std::endl;
	std::cout << "/disable_maintenance (disable server maintenance)" << std::endl;
	std::cout << "/exit (clean server exit)" << std::endl;
	std::cout << "/block_connection <ip> (block ip to prevent dos attacks)" << std::endl;
	std::cout << "/unblock_connection <ip> (unblock ip)" << std::endl;
	std::cout << "/show_blocked_connections (show all blocked ips)" << std::endl;
}

void cmdServerExit(const std::vector<std::string>& input)
{
	// cleanup server
	cleanupServer();
}

void cmdBlockConnection(const std::vector<std::string>& input)
{
	boost::system::error_code ec;
	boost::asio::ip::address address = boost::asio::ip::address::from_string(input.at(1), ec);
	if(ec)
	{
		std::cout << ec.message() << std::endl;
	} else
	{
		boost::optional<int> found;
		soci::session sql(Nostale::Network::globalDatabase->getPool());
		sql << "SELECT ID FROM blocked_ips WHERE IP = :ip", soci::use(input.at(1)), soci::into(found);
		if(!found.is_initialized())
		{
			sql << "INSERT INTO blocked_ips (IP) VALUES(:ip)", soci::use(input.at(1));
			std::cout << "IP ('" << input.at(1) << "') successfully blocked." << std::endl;
			server->addConnection(address);
		} else
		{
			std::cout << "IP ('" << input.at(1) << "') already blocked." << std::endl;
		}
	}
}

void cmdUnblockConnection(const std::vector<std::string>& input)
{
	boost::system::error_code ec;
	boost::asio::ip::address address = boost::asio::ip::address::from_string(input.at(1), ec);
	if(ec)
	{
		std::cout << ec.message() << std::endl;
	} else
	{
		boost::optional<int> found;
		soci::session sql(Nostale::Network::globalDatabase->getPool());
		sql << "SELECT ID FROM blocked_ips WHERE IP = :ip", soci::use(input.at(1)), soci::into(found);
		if(found.is_initialized())
		{
			sql << "DELETE FROM blocked_ips WHERE ID = :ip", soci::use(found.get());
			std::cout << "IP ('" << input.at(1) << "') successfully unblocked." << std::endl;
			server->removeConnection(address);
		} else
		{
			std::cout << "IP ('" << input.at(1) << "') is not blocked." << std::endl;
		}		
	}
}

void cmdShowBlockedConnections(const std::vector<std::string>& input)
{
	soci::session sql(Nostale::Network::globalDatabase->getPool());
	soci::rowset<std::string> records = sql.prepare << ("SELECT IP FROM blocked_ips");
	for(auto it = records.begin(); it != records.end(); ++it)
		std::cout << "IP('" << *it << "')" << std::endl;
}

void setServerConnectionFilter(void)
{
	std::vector<boost::asio::ip::address> connectionFilter;
	soci::session sql(Nostale::Network::globalDatabase->getPool());
	soci::rowset<std::string> records = sql.prepare << ("SELECT IP FROM blocked_ips");
	for(auto it = records.begin(); it != records.end(); ++it)
	{
		boost::system::error_code ec;
		boost::asio::ip::address address = boost::asio::ip::address::from_string(*it, ec);
		if(!ec)
			connectionFilter.push_back(address);
	}

	server->setConnectionFilter(connectionFilter);
}