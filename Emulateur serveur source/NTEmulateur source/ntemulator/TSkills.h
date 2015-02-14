#pragma once

#define SKILL_NOTFOUND -1

#ifndef _H_SKILLS
#define _H_SKILLS

//Typedefintionen
class TSkills;
extern TSkills Skills;
//----------------------------

//Strukturen
struct structEnemySkills
{
	int vnum;
	int typ;
	int effect;
	int moveid;
	int mpcost;
	int range;
	int casttime;
	int cooldown;
};

struct structSkills
{
	int vnum;
	int typ;
	int castid;
	int effect;
	int moveid;
	int mpcost;
	int jlevel;
	int cp;
	int range;
	int target;
	int casttime;
	int cooldown;
	int cost;
	int itemneed;
};

//-----------------------------

class TSkills
{
public:
	TSkills(void); //Konstruktor
	~TSkills(void); //Destruktor

	std::vector<structSkills> Skills; //Skills Array
	std::vector<structEnemySkills> EnemySkills; //Gegner Skills
	void Player_Cast(TSocket *Client, int castid, int targetobj, int targetid);
	void Player_CheckCast(int mapindex, int player_all);
	void Hit_Enemy(TSocket *Client, int mapindex, int allocated_en );
	int Find_EnemySkill_allocated(int vnum);
};

#endif _H_SKILLS
