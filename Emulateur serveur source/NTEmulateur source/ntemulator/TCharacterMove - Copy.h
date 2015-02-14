#pragma once

#ifndef _H_CHARACTERMOVE
#define _H_CHARACTERMOVE

//Typedefinition

class TSocket;
class TCharacterMove;
extern TCharacterMove CharacterMove;

//----------------

class TCharacterMove
{
public:
	TCharacterMove(void); //Konstruktor
	~TCharacterMove(void); //Dekonstruktor

	void Move_Character(TSocket *Client, short x, short y, short z, short speed); //Character Bewegung überprüfen
};

#endif _H_CHARACTERMOVE