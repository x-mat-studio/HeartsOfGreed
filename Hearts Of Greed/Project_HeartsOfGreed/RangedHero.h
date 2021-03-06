#ifndef __RANGED_HERO_H__
#define __RANGED_HERO_H__

#include "Hero.h"



class RangedHero : public Hero
{
public:
	RangedHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
		Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
		Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
		Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& tileOnWalk, 
		Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
		HeroStats& stats, Skill& skill1, Skill& passiveSkill);

	RangedHero(fMPoint position, RangedHero* copy, ENTITY_ALIGNEMENT alignement);

	bool Start(SDL_Texture* texture);

	bool ActivateSkill1(fMPoint clickPosition);
	bool ActivateSkill2();
	bool ActivateSkill3();

	bool PreProcessSkill1();
	bool PreProcessSkill2();
	bool PreProcessSkill3();

	bool ExecuteSkill1();
	bool ExecuteSkill2();
	bool ExecuteSkill3();

	void Attack();

	void LevelUp();

	void BlitCommandVfx(Frame& currframe, int alphaValue);

	void PlayGenericNoise(int random);
	void PlayOnHitSound();

	Skill GetPassiveSkill() const;
	void ReplacePassiveSkill(Skill& skill);

private:
	void UnleashParticlesSkill1(float posx, float posy);

public:

	fMPoint skill1PosLaunch;
	skillArea* skill1Area;
	Skill passiveSkill;

	//This is a placeholder for particles
	SDL_Rect explosionRect;
	Animation* currentVfx;
	

};


#endif //__RANGED_HERO_H__
