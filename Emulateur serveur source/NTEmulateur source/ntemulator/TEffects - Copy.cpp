#include "stdafx.h"
#include "TEffects.h"

TEffects Effects;
//------------------------------------------------------------

TEffects::TEffects(void)
{
}

//------------------------------------------------------------

TEffects::~TEffects(void)
{
}

//------------------------------------------------------------

void TEffects::ShowEffect( TSocket *Client, int effectid )
{
	std::stringstream ss;
	ss << "eff 1 " << Client->Character->CharacterInfo.ID << " " << effectid;

	int mapindex = Map.GetMapIndex(Client->Character->Map.id);

	Map.SendMap(mapindex, NULL, true, ss.str());
}

//------------------------------------------------------------