#pragma once

#ifndef _H_NPCS
#define _H_NPCS

//Typedefintionen
class TSocket;
class TNpc;
extern TNpc Npcs;
//----------------------------

//Strukturen

struct structNpcs
{
	int level;
	int vnum;
	int hp;
	int mp;
};

//-----------------------------

class TNpc
{

public:
	TNpc(void); //Konstruktor
	~TNpc(void); //Destruktor

	std::vector<structNpcs> Npcs; //Npc Array

	void OpenNpcDialog(TSocket *Client, int npc_id); //NPC Dialog öffnen
	void GetTargetStatus(TSocket *Client, int type, int id); //NPC Status [HP,MP etc] senden
};

#endif _H_NPCS
