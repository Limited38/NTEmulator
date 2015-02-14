#pragma once

#define ITEM_NOT_FOUND -1
//Spezielle Items
#define SITEM_GOLD 1046

//Settings
#define ITEM_DROPBOUND_TIME 30
#define ITEM_LIFE_TIME 300

#ifndef _H_ITEMS
#define _H_ITEMS

//Typedefintionen

class TItems;
extern TItems Items;

//--------------------------

//Strukturen

struct structItems
{
	std::string name;
	int vnum;
	int inventorytype;
	int price;
	std::vector<int> effects;
	bool on_use_delete;
};

//------------------------

class TItems
{

public:
	TItems(void); //Konstruktor
	~TItems(void); //Destruktor

	std::vector<structItems> vItems; //Item Array

	int MakeItem(TSocket *Client, int itemid); //Item benutzen
	int GetItemIndex(int vnum); //Item Index returnen
	void Bound_Check(int mapindex, int itemindex); //
	void Lifetime_Check(int mapindex, int itemindex); //
	std::mutex lockthread;
	static DWORD WINAPI ItemsThread(LPVOID lpdwThreadParam);
};

#endif _H_ITEMS
