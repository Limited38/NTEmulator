#include "stdafx.h"
#include "TLogin.h"

TLogin Login;
//------------------------------------------------------------

TLogin::TLogin()
{
}

//------------------------------------------------------------

TLogin::~TLogin()
{
}

//------------------------------------------------------------

void TLogin::VerifyAccount( TSocket *Client, std::string user, std::string password )
{
	this->QueryAccount(Client, user, password);
}

//------------------------------------------------------------

void TLogin::QueryServerlist( TSocket *Client, int accountid)
{
	int session = General.CreateRandomNumber(10000, 50000);
	std::stringstream ss;
	ss << session;

	//Serverpacket an den Clienten senden
	Client->Send("NsTest "+ ss.str() +" 127.0.0.1:1337:0:1.0.nosXplode:10000.10000.0");

	//Statement in der Datenbank freigeben

	//Account Datenbank abfrage
	try
	{
		Database.stmt = Database.connection->prepareStatement("UPDATE tbl_accounts SET LastSession = ?, LastLoginIP = ?, LastLoginDate = ? WHERE AccountID = ?");
		Database.stmt->setInt(1, session);
		Database.stmt->setString(2, inet_ntoa(Client->clientaddr.sin_addr));
		Database.stmt->setString(3, General.GetDateTime());
		Database.stmt->setInt(4, accountid);
		Database.stmt->executeUpdate();

	} catch(sql::SQLException &e)
	{
		std::cout << "ERROR SQL Exception: " << e.getSQLStateCStr() << std::endl;
	}
}

//------------------------------------------------------------

bool TLogin::QueryAccount( TSocket *Client, std::string user, std::string password )
{
	//Account Datenbank Abfrage
	try
	{
		Database.stmt = Database.connection->prepareStatement("SELECT * FROM tbl_accounts WHERE User = ? AND Password = ?");
		Database.stmt->setString(1, user);
		Database.stmt->setString(2, password);
		Database.result = Database.stmt->executeQuery();
	} catch(sql::SQLException &e)
	{
		std::cout << "ERROR SQL Exception: " << e.getSQLStateCStr() << std::endl;
	}
	
	//Wenn Keine Rows returned, Account nicht vorhanden
	if(Database.result->rowsCount() == 0)
	{
		Client->Send("fail Invalid Account details, please try again");
		return 1;
	}

	//Datenkbank Result's überprüfen
	while(Database.result->next())
	{
		if(Database.result->getBoolean("IsBlocked") == 1) //Prüfen ob Account geblockt
		{
			Client->Send("fail Your Account is blocked");
			return 1;

		} else if(Database.result->getBoolean("IsOnline") == 1) //Prüfen ob Account bereits online
		{
			Client->Send("fail Your Account is already online");
			return 1;
		} else
		{
			//Serverliste senden, wenn alles ok
			this->QueryServerlist(Client,  Database.result->getInt("AccountID"));
			return 0;
		}
	} 
}

//------------------------------------------------------------

