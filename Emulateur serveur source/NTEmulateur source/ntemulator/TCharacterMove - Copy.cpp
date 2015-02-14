#include "stdafx.h"
#include "TCharacterMove.h"

TCharacterMove CharacterMove;
//------------------------------------------------------------

TCharacterMove::TCharacterMove(void)
{
}

//------------------------------------------------------------

TCharacterMove::~TCharacterMove(void)
{
}

//------------------------------------------------------------

void TCharacterMove::Move_Character(TSocket *Client, short x, short y, short z, short speed)
{
	std::stringstream ss;
	int MapIndex = Map.GetMapIndex(Client->Character->Map.id);
	bool isObjectZone = Map.isBlockedZone(MapIndex, x, y);
	int MapPlayersSize = Map.MapData[MapIndex].Players.size();
	int distance = Math.Distance(Client->Character->Map.x, x, Client->Character->Map.y, y);
	//Prüfen ob Koordinaten, Zones etc stimmen
	if(Client->Character->Map.speed != speed || distance > 8 || isObjectZone == true)
	{
		Client->Disconnect();
		return;
	} else
	{
		Client->Character->Map.x = x;
		Client->Character->Map.y = y;
		CharacterInfo.Send_CharacterSpeed(Client);
	}

	ss << "mv 1 " << Client->Character->CharacterInfo.ID << " " << x << " " << y << " " << speed;
	Map.SendMap(MapIndex, Client->Character->CharacterInfo.ID, false, ss.str());

	return;
}

//------------------------------------------------------------