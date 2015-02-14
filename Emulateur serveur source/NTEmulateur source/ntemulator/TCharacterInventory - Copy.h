#pragma once
#define AUTODETECT_SLOT -1
#define AUTODETECT_INVENTORY -1
#define SLOTS_FULL -2
#define UNKNOWN_INVENTORY -2
#define MAX_SLOTS 45
#define INVENTORY_SIZE 3
#define EQUIP_INVENTORY 0
#define MAIN_INVENTORY 1
#define OTHER_INVENTORY 2
#define GOLD_INVENTORY 99

#ifndef _H_CHARACTERINVENTORY
#define _H_CHARACTERINVENTORY

//Typedefinitionen

class TGeneral;
class TCharacterInventory;
extern TCharacterInventory Inventory;

//Strukturen, Klassen

struct structInventory
{
	bool inUse;
	int id;
	int count;
	int rare;
	int upgrade;
};

struct structTradeInventory
{
	int inv;
	int slot;
	int amount;
};

struct structCharacterInventory
{
	bool Initialized;
	bool BackpackActive;
	int gold_count;
	std::vector<std::vector<structInventory>> Index;
	std::vector<structTradeInventory> TradeInventory;
	int gold_trade;
	bool trade_ready;
	bool trade_finish;
};

//-------------------------------------------

class TCharacterInventory
{
private:
	void SetItemFromDatabase(TSocket *Client, int characterslot, int inventorytype, int slot, int id, int count, int rare, int upgrade); //Mini Wrapper
	int DetectSlot(TSocket *Client, int inventorytype, int slot); //Slot ermitteln oder manuell setzen
	int DetectInventory(TSocket *Client, int inventorytype, int itemid); //Item Inventar Type ermitteln oder manuell setzen
public:
	TCharacterInventory(void); //Konstruktor
	~TCharacterInventory(void); //Destruktor

	void AddItem(TSocket *Client, int inventorytype, int slot, int id, int count, int rare, int upgrade); //Item im Inventar hinzufügen
	void RemoveItem(TSocket *Client, int inventorytype, int slot, int amount = -1); //Item im Inventar entfernen
	void ParseInventory(TSocket *Client, int characterslot, int inventorytype, std::string inventoryarray); // Datenbank Infos parsen
	void RemoveItemRequest(TSocket *Client, int inventorytype, int slot); //Anfrage um Item zu löschen
	void MoveItem(TSocket *Client, int inventorytype, int slot, int count, int newslot); //Item im Inventar verschieben
	void UseItem(TSocket *Client, int inventorytype, int slot); //Item im Inventar nutzen
	void DropItem(TSocket *Client, int inventory, int slot, int amount); //Item droppen
	void GetItem(TSocket *Client, int action, int charid, int itemid); //item aufheben
};

#endif _H_CHARACTERINVENTORY
