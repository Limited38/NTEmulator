#pragma once

#ifndef _H_CHARACTERINFO
#define _H_CHARACTERINFO

//Typedefinitionen

class TSocket;
class TPacketCypher;
class TCharacterInfo;
extern TCharacterInfo CharacterInfo;

//----------------------

//Strukturen

struct structCharacterGroup
{
	bool ingroup;
	int groupid;
	std::vector<int> requests;
};

struct structPing
{
	int packetcount;
	int lastpulsetime;
	int lastpulsecount;
	int lastautopulse;
};

struct structCharacterSkills
{
	int vnum;
	int castid;
	int cooldown;
};

struct structCharacterInfo
{
	int ID;
	int Slot;
	std::string Name;
	bool Gender;
	int Hairstyle;
	int Haircolor;
	int ClassType;
};

struct structCharacterStatus
{
	int currenthp;
	int currentmp;
	int maxhp;
	int maxmp;
	bool alive;
};

struct structCharacterLevel
{
	int mainlevel;
	int secondlevel;
	int mainexp;
	int mainexp_levelup;
	int secondexp;
	int secondexp_levelup;
};

struct structCharacterCast
{
	bool iscasting;
	int casttime;
	int allocated_id;
	int allocated_cast;
	int targetid;
	int targetobj;
};
//---------------------------

class TCharacterInfo
{
public:
	TCharacterInfo(void); //Konstruktor
	~TCharacterInfo(void); //Destruktor

	void GetAllMapPackets(TSocket *Client);
	void Send_CharacterInventory(TSocket *Client); //Character Inventar laden
	void Send_CharacterInfo(TSocket *Client); //Character Normale Informationen senden
	void Send_CharacterSkills(TSocket *Client); //Character Skills Informationen senden
	void Send_CharacterMap(TSocket *Client); //Character Map Informationen senden
	void Send_CharacterSpeed(TSocket *Client); //Character Geschwindigkeits Informationen senden
	void Send_CharacterMapout(TSocket *Client); //Character Map verlassen senden
	void Send_PlayerIn(TSocket *Player, TSocket *Client); //Beitritt eines Spielers auf der Map senden
	void Send_PlayerOut(TSocket *Player, TSocket *Client); //Verlassen eines Spielers auf der Map senden
	void Save_Character(TSocket *Client); //Charakterdaten in der Datenbank speichern
	void Send_CharacterStats(TSocket *Client); //Character Status senden 
	void Send_CharacterGold(TSocket *Client); //Charakter Gold Senden
	void Send_CharacterLevel(TSocket *Client); //Charakter Level Senden

	void XP_Add(TSocket *Client, int mainxp, int secondxp, int spxp); //XP geben

	static DWORD WINAPI PlayerThread(LPVOID lpdwThreadParam);
	std::mutex lockthread;
};

#endif _H_CHARACTERINFO