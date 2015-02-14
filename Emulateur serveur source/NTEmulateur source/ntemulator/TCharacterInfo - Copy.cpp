#include "stdafx.h"
#include "TCharacterInfo.h"

TCharacterInfo CharacterInfo;
//------------------------------------------------------------

TCharacterInfo::TCharacterInfo(void)
{
}

//------------------------------------------------------------

TCharacterInfo::~TCharacterInfo(void)
{
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterInfo( TSocket *Client )
{
	//Packet erstellen und senden
	std::stringstream ss;
	ss << "c_info " << Client->Character->CharacterInfo.Name << " - -1 -1 - " << Client->Character->CharacterInfo.ID << " 1 "
		<< Client->Character->CharacterInfo.Gender << " " << Client->Character->CharacterInfo.Hairstyle << " "
		<< Client->Character->CharacterInfo.Haircolor << " 0 1 0 0 0";
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterSkills( TSocket *Client )
{
	//Packet erstellen und senden
	std::stringstream ss;
	int SkillsSize = Client->Character->CharacterSkills.size();
	ss << "ski 200 201"; //TODO: Primärer und Sekundärer Skill

	for(int i = 0; i < SkillsSize; i++)
	{
		ss << " " << Client->Character->CharacterSkills[i].vnum;
	}

	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterMap( TSocket *Client )
{
	//Packets erstellen und senden
	std::stringstream ss;

	ss << "at " << Client->Character->CharacterInfo.ID << " " << Client->Character->Map.id << " "
		<< Client->Character->Map.x << " " << Client->Character->Map.y << " 0 0 1 0";
	Client->Send(ss.str());

	ss.str("");
	ss << "c_map 0 " << Client->Character->Map.id << " 0";
	Client->Send(ss.str());

	this->Send_CharacterSpeed(Client);

	//Send Info Stuff
	Map.GetPortalsOnMap(Client);
	Map.GetNpcsOnMap(Client);
	Map.GetItemsOnMap(Client);
	Map.GetEnemiesOnMap(Client);
}

//------------------------------------------------------------

void TCharacterInfo::Send_PlayerOut( TSocket *Player, TSocket *Client )
{
	std::stringstream ss;
	ss << "out 1 " << Player->Character->CharacterInfo.ID;
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_PlayerIn( TSocket *Player, TSocket *Client )
{
	//Packets erstellen und senden
	std::stringstream ss;
	ss << "in 1 " << Player->Character->CharacterInfo.Name << " - " << Player->Character->CharacterInfo.ID << " "
		<< Player->Character->Map.x << " " << Player->Character->Map.y << " 1 0 " << Player->Character->CharacterInfo.Gender << " "
		<< Player->Character->CharacterInfo.Hairstyle << " " << Player->Character->CharacterInfo.Haircolor << " "
		<< "0 -1.-1.-1.-1.-1.-1.-1.-1 100 100 0 -1 0 0 0 0 0 0 0 0 -1 - 1 0 0 0 0 1 0";
	Client->Send(ss.str());

	ss.str("");
	ss << "in 1 " << Client->Character->CharacterInfo.Name << " - " << Client->Character->CharacterInfo.ID << " "
		<< Client->Character->Map.x << " " << Client->Character->Map.y << " 1 0 " << Client->Character->CharacterInfo.Gender << " "
		<< Client->Character->CharacterInfo.Hairstyle << " " << Client->Character->CharacterInfo.Haircolor << " "
		<< "0 -1.-1.-1.-1.-1.-1.-1.-1 100 100 0 -1 0 0 0 0 0 0 0 0 -1 - 1 0 0 0 0 1 0";
	Player->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Save_Character(TSocket *Client)
{
	//TServer -> Playerdisconnect -> Save Character
	//TServer -> Gewisse Zeit -> Auto Save Character
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterMapout( TSocket *Client )
{
	//Packet erstellen und senden
	std::stringstream ss;
	ss << "mapout";
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterSpeed( TSocket *Client )
{
	std::stringstream ss;
	ss << "cond 1 " << Client->Character->CharacterInfo.ID << " 0 0 " << Client->Character->Map.speed;
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterInventory( TSocket *Client )
{
	std::stringstream equip, main, other;
	equip << "inv 0";
	main << "inv 1";
	other << "inv 2";

	//Anzahl Freier Slots im Inventar laden
	if(Client->Character->Inventory.BackpackActive)
	{
		Client->Send("exts 45 45 45");
	} else
	{
		Client->Send("exts 32 32 32");
	}

	//Alle Inventar Items ins Packet laden

	for(auto i = 0; i < MAX_SLOTS; i++)
	{
		// 0 = EQUIP, 1 = MAIN, 2 = OTHER
		if(Client->Character->Inventory.Index[0][i].inUse == true)
		{
			equip << " " << i << "." << Client->Character->Inventory.Index[0][i].id << "." << Client->Character->Inventory.Index[0][i].rare << "." << Client->Character->Inventory.Index[0][i].upgrade;
		}

		if(Client->Character->Inventory.Index[1][i].inUse == true)
		{
			main << " " << i << "." << Client->Character->Inventory.Index[1][i].id << "." << Client->Character->Inventory.Index[1][i].count;
		}

		if(Client->Character->Inventory.Index[2][i].inUse == true)
		{
			other << " " << i << "." << Client->Character->Inventory.Index[2][i].id << "." << Client->Character->Inventory.Index[2][i].count;
		}
	}
	

	//Inventar Packets senden
	Client->Send(equip.str());
	Client->Send(main.str());
	Client->Send(other.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterStats( TSocket *Client )
{
	std::stringstream ss;
	ss << "stat " << Client->Character->Status.currenthp << " " << Client->Character->Status.maxhp << 
		" " << Client->Character->Status.currentmp << " " << Client->Character->Status.maxmp << " 0 1024";
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterGold( TSocket *Client )
{
	std::stringstream ss;
	ss << "gold " << Client->Character->Inventory.gold_count << " 0";
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::GetAllMapPackets( TSocket *Client )
{
	CharacterInfo.Send_CharacterInfo(Client);
	CharacterInfo.Send_CharacterStats(Client);
	CharacterInfo.Send_CharacterLevel(Client);
	CharacterInfo.Send_CharacterMap(Client);
	CharacterInfo.Send_CharacterSkills(Client);
	CharacterInfo.Send_CharacterGold(Client);
	CharacterInfo.Send_CharacterInventory(Client);
	Chat.LoadSystemMessages(Client);

	Map.AddPlayerToMap(Client);
}

//------------------------------------------------------------

void TCharacterInfo::Send_CharacterLevel( TSocket *Client )
{
	std::stringstream ss;
	
	ss << "lev " << Client->Character->Level.mainlevel << " " << Client->Character->Level.mainexp
		<< " " << Client->Character->Level.secondlevel << " " << Client->Character->Level.secondexp
		<< " " << Client->Character->Level.mainexp_levelup << " " << Client->Character->Level.secondexp_levelup
		<< " 0 0";
	Client->Send(ss.str());
}

//------------------------------------------------------------

void TCharacterInfo::XP_Add(TSocket *Client, int mainxp, int secondxp, int spxp)
{
	Client->Character->Level.mainexp += mainxp * Config.Rate_XP;
	Client->Character->Level.secondexp += secondxp * Config.Rate_JXP;

	//level Up
	if(Client->Character->Level.mainexp >= Client->Character->Level.mainexp_levelup)
	{
		Client->Character->Level.mainexp = Client->Character->Level.mainexp - Client->Character->Level.mainexp_levelup;
		Client->Character->Level.mainlevel += 1;
		Client->Character->Level.mainexp_levelup = 300 * Client->Character->Level.mainlevel;

		int MapIndex = Map.GetMapIndex(Client->Character->Map.id);

		std::stringstream message;
		message << "msg 0 Your combat level has been increased.";
		Client->Send(message.str());
		message.str("");
		message << "eff 1 " << Client->Character->CharacterInfo.ID << " 6";
		Map.SendMap(MapIndex, NULL, true, message.str());
		message.str("");
		message << "eff 1 " << Client->Character->CharacterInfo.ID << " 198";
		Map.SendMap(MapIndex, NULL, true, message.str());

		Client->Character->Status.currenthp = Client->Character->Status.maxhp;
		Client->Character->Status.currentmp = Client->Character->Status.maxmp;
		CharacterInfo.Send_CharacterStats(Client);
	}
	//level Up
	if(Client->Character->Level.secondexp >= Client->Character->Level.secondexp_levelup)
	{
		Client->Character->Level.secondexp = Client->Character->Level.secondexp - Client->Character->Level.secondexp_levelup;
		Client->Character->Level.secondlevel += 1;
		Client->Character->Level.secondexp_levelup = 2200 * Client->Character->Level.secondlevel;

		std::stringstream message;
		message << "msg 0 Your job level has been increased.";
		Client->Send(message.str());

		Client->Character->Status.currenthp = Client->Character->Status.maxhp;
		Client->Character->Status.currentmp = Client->Character->Status.maxmp;
		CharacterInfo.Send_CharacterStats(Client);
	}
	this->Send_CharacterLevel(Client);
}

//------------------------------------------------------------

DWORD WINAPI TCharacterInfo::PlayerThread(LPVOID lpdwThreadParam)
{
	Sleep(100);

	std::cout << "TCharacterInfo::PlayerThread() running" << std::endl;
	do 
	{
		int mapsize = Map.MapData.size();
		int playersize;

		for(int i = 0; i < mapsize; i++)
		{
			CharacterInfo.lockthread.lock();

			playersize = Map.MapData[i].Players.size();

			for(int t = 0; t < playersize; t++)
			{
				Skills.Player_CheckCast(i,t);
			}

			CharacterInfo.lockthread.unlock();
		}
		Sleep(7);
	} while (1);
}