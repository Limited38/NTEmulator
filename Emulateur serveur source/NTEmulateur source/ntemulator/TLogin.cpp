#include "stdafx.h"
#include "TLogin.h"

//------------------------------------------------------------

TLogin::TLogin()
{
	this->General = new TGeneral;
}

//------------------------------------------------------------

TLogin::~TLogin()
{
	delete this->General;
}

//------------------------------------------------------------

void TLogin::VerifyAccount( TSocket *Client, std::string user, std::string password )
{
	TDatabase *currentConnection = 0; 

	//Datenbank Connection vom Pool anfragen
	do 
	{
		currentConnection = Database.GetConnectionFromPool();
	} while (currentConnection == 0);

	//Account überprüfen
	this->QueryAccount(Client, currentConnection, user, password);

	//Datenbank pool verlassen
	Database.Quit(currentConnection);
}

//------------------------------------------------------------

void TLogin::QueryServerlist( TSocket *Client, TDatabase *currentConnection, int accountid)
{
	int session = this->General->CreateRandomNumber(10000, 50000);
	std::stringstream ss;
 	ss << session;

	//Serverpacket an den Clienten senden
	Client->Send("NsTest "+ ss.str() +" 127.0.0.1:1337:0:1.0.nosXplode:10000.10000.0");

	//Statement in der Datenbank freigeben
	delete currentConnection->stmt;

	//Account Datenbank abfrage
	try
	{
		currentConnection->stmt = currentConnection->connection->prepareStatement("UPDATE tbl_accounts SET LastSession = ?, LastLoginIP = ?, LastLoginDate = ? WHERE AccountID = ?");
		currentConnection->stmt->setInt(1, session);
		currentConnection->stmt->setString(2, inet_ntoa(Client->clientaddr.sin_addr));
		currentConnection->stmt->setString(3, this->General->GetDateTime());
		currentConnection->stmt->setInt(4, accountid);
		currentConnection->stmt->executeUpdate();

	} catch(sql::SQLException &e)
	{
		std::cout << "ERROR SQL Exception: " << e.getSQLStateCStr() << std::endl;
	}
}

//------------------------------------------------------------

void TLogin::QueryAccount( TSocket *Client, TDatabase *currentConnection, std::string user, std::string password )
{
	//Account Datenbank Abfrage
	try
	{
		currentConnection->stmt = currentConnection->connection->prepareStatement("SELECT * FROM tbl_accounts WHERE User = ? AND Password = ?");
		currentConnection->stmt->setString(1, user);
		currentConnection->stmt->setString(2, password);
		currentConnection->result = currentConnection->stmt->executeQuery();
	} catch(sql::SQLException &e)
	{
		std::cout << "ERROR SQL Exception: " << e.getSQLStateCStr() << std::endl;
	}

	//Wenn Keine Rows returned, Account nicht vorhanden
	if(currentConnection->result->rowsCount() == 0)
	{
		Client->Send("fail Invalid Account details, please try again");
		return;
	}

	//Datenkbank Result's überprüfen
	while(currentConnection->result->next())
	{
		if(currentConnection->result->getBoolean("IsBlocked") == 1) //Prüfen ob Account geblockt
		{
			Client->Send("fail Your Account is blocked");
			return;

		} else if(currentConnection->result->getBoolean("IsOnline") == 1) //Prüfen ob Account bereits online
		{
			Client->Send("fail Your Account is already online");
			return;
		} else
		{
			//Serverliste senden, wenn alles ok
			this->QueryServerlist(Client, currentConnection, currentConnection->result->getInt("AccountID"));
			return;
		}
	} 
}

//------------------------------------------------------------

