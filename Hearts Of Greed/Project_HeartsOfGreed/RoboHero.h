#ifndef __ROBO_HERO_H__
#define __ROBO_HERO_H__

#include "Hero.h"


class RoboHero : public Hero
{
public:
	RoboHero(fMPoint position, Collider* col, Animation& walkLeft, Animation& walkLeftUp, Animation& walkLeftDown, Animation& walkRightUp,
		Animation& walkRightDown, Animation& walkRight, Animation& idleRight, Animation& idleRightDown, Animation& idleRightUp, Animation& idleLeft,
		Animation& idleLeftUp, Animation& idleLeftDown, Animation& punchLeft, Animation& punchLeftUp, Animation& punchLeftDown, Animation& punchRightUp,
		Animation& punchRightDown, Animation& punchRight, Animation& skill1Right, Animation& skill1RightUp, Animation& skill1RightDown, Animation& skill1Left,
		Animation& skill1LeftUp, Animation& skill1LeftDown, Animation& deathRight, Animation& deathRightUp, Animation& deathRightDown, Animation& deathLeft, Animation& deathLeftUp, Animation& deathLeftDown, 
		Animation& tileOnWalk, HeroStats& stats, Skill& skill1, Skill& passiveSkill);

	RoboHero(fMPoint position, RoboHero* copy, ENTITY_ALIGNEMENT alignement);

	~RoboHero();

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

	void LevelUp();

	void PlayGenericNoise(int random);

	Skill GetPassiveSkill() const;
	void ReplacePassiveSkill(Skill& skill);

private:

	void Attack();
	void UpdatePasiveSkill(float dt);

	void ResetBuff();
	void ApplyBuff();

	void BlitCommandVfx(Frame& currframe, int alphaValue);

	void UnleashParticlesSkill1(float posx, float posy);


private:

	Skill passiveSkill;
	
	int acumulations;
	float timer;

	float currentDamage;
	float currentSpeed;
};


#endif //__ROBO_HERO_H__
