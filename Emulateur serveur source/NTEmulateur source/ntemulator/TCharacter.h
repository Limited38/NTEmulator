#pragma once

#define PING_BUFFER 1 //1 Sekunde Puffer beim Pulse senden
#define PING_AUTO_BUFFER 120 // Seit 2 Minuten kein Pulse Packet mehr -> Auto Kick

#ifndef _H_CHARACTER
#define _H_CHARACTER

//Typedefinitionen

class TSocket;
struct structCharacterInfo;
struct structCharacterMap;
struct structCharacterInventory;
struct structCharacterGroup;
struct structPing;
struct structFriends;
struct structCharacterQuicklist;
struct structEquipmentSlot;

class TCharacter;
struct structCharacterStatus;
extern TCharacter Character;
//---------------------

//Strukturen

struct structCharacterList
{
	bool isReceived;
	std::vector<structCharacterQuicklist> Quicklist;
	structCharacterInfo CharacterInfo;
	structCharacterStatus Status;
	structCharacterLevel Level;
	structCharacterGroup Group;
	std::vector<structCharacterSkills> CharacterSkills;
	structCharacterMap Map;
	structCharacterInventory Inventory;
	std::vector<structEquipmentSlot> Equipment;
	structPing Ping;
	std::vector<structFriends> Friends;
	std::vector<int> FriendRequests;
	std::vector<int> TradeRequests;
	structCharacterCast Cast;
	structCharacterStats Stats;

	bool trade_active;
	int active_trade_id;
};

//----------------------

class TCharacter
{
	void ExecuteCharacterList(TSocket *Client); //Datenbank Charakterliste abfragen
public:
	TCharacter(void); //Konstruktor
	~TCharacter(void); //Destruktor

	void LoadCharacter(TSocket *Client); //Charaktere Laden
	void NewCharacter(TSocket *Client, std::string name, int slot, bool gender, int hairstyle, int haircolor); //Charakter erstellen
	void DeleteCharacter(TSocket *Client, int slot, std::string password); //Charakter löschen
	void SelectCharacter(TSocket *Client, int slot); //Charakter auswählen
	void Pulse_Activate( TSocket *Client ); //Pulse überprüfung Aktivieren
	void Pulse_Check( TSocket *Client, int pulse); //Pulse wert überprüfen
	void Pulse_Auto_Check(TSocket *Client); //Pulse wert überprüfen
};

#endif _H_CHARACTER