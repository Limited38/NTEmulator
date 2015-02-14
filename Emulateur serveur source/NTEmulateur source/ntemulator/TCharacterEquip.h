#pragma once


#ifndef _H_CHARACTEREQUIP
#define _H_CHARACTEREQUIP

struct structEquipInventory
{
	int mainweapon;
	int secondweapon;
	int armour;
	int head;
	int gloves;
	int shoes;
	int chain;
	int ring;
	int armlet;
	int mask;
	int fairy;
	int talisman;
	int sp;
	int suit;
	int head_suit;
};



class TCharacterEquip
{
public:
	TCharacterEquip(void);
	~TCharacterEquip(void);
};

#endif _H_CHARACTEREQUIP