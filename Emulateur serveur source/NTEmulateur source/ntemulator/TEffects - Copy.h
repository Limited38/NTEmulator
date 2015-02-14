#pragma once

#ifndef _H_EFFECTS
#define _H_EFFECTS

class TEffects;
extern TEffects Effects;

class TEffects
{
public:
	TEffects(void);
	~TEffects(void);

	void ShowEffect(TSocket *Client, int effectid);
};

#endif _H_EFFECTS

