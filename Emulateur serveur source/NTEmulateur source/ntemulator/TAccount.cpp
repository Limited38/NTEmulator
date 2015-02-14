#include "stdafx.h"
#include "TAccount.h"

TAccount Account;
//------------------------------------------------------------

bool TAccount::Check( TSocket *Client)
{
	//Account Datenbank abfrage 
	try
	{
		Database.stmt = Database.connection->prepareStatement("SELECT * FROM tbl_accounts WHERE User = ? and Password = ?");
		Database.stmt->setString(1, Client->Account->user);
		Database.stmt->setString(2, Client->Account->password);
		Database.result = Database.stmt->executeQuery();

		//Wenn keine Rows empfangen, Disconnect
		if(Database.result->rowsCount() == 0)
		{
			Client->Disconnect();
			return 1;
		}

		//Datenbank result überprüfen
		while(Database.result->next())
		{
			Client->Account->accountid = Database.result->getInt("AccountID"); //AccountID saven

			if(Database.result->getInt("LastSession") != Client->Account->session) //Wenn Session nicht übereinstimmt, Disconnect
			{
				Client->Disconnect();
				return 1;
			} else if(Database.result->getBoolean("IsBlocked") == 1) //Wenn Blocked, Disconnect
			{
				Client->Disconnect();
				return 1;
			} else if(Database.result->getBoolean("IsOnline") == 1) //Wenn bereits Online, Disconnect
			{
				Client->Disconnect();
				return 1;
			}
		}
	} catch(sql::SQLException &e)
	{
		std::cout << "ERROR SQL Exception: " << e.getSQLStateCStr() << std::endl;
	}

	return 0;
}

//------------------------------------------------------------
