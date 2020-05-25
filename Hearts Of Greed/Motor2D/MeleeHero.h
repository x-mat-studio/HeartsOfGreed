#ifndef __MELEE_HERO_H__
#define __MELEE_HERO_H__

#include "Hero.h"


class MeleeHero : public Hero
{
public:
	MeleeHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
		Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
		Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
		Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& tileOnWalk,
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
		HeroStats& stats, Skill& skill1, Skill& passiveSkill);

	MeleeHero(fMPoint position, MeleeHero* copy, ENTITY_ALIGNEMENT alignement);

	~MeleeHero();


	bool ActivateSkill1(fMPoint clickPosition);
	bool ActivateSkill2();
	bool ActivateSkill3();

	bool PreProcessSkill1();
	bool PreProcessSkill2();
	bool PreProcessSkill3();

	bool ExecuteSkill1();
	bool ExecuteSkill2();
	bool ExecuteSkill3();

	void LevelUp();

	void PlayGenericNoise(int random);

private:
	void BlitCommandVfx(Frame& currframe, int alphaValue);
	void UpdatePasiveSkill(float dt);

private:
	Skill passiveSkill;


};


#endif //__MELEE_HERO_H__