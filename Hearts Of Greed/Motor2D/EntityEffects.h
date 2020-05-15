#include "Entity.h"

struct SkillEffect
{
	SkillEffect();

	float time;
	float severity;
	int statTaken;
};

struct EntityEffects
{
	EntityEffects();

public:
	void UpdateEffects();

	void DeleteEffect(SKILL_EFFECT& effect);
	void DeleteAllEffects();

	void AddNewEffect(SKILL_EFFECT& effect, float time, float severity = 1.0f);

private:

	void UpdateEffect(SKILL_EFFECT effect);

private:

	SkillEffect effects[SKILL_EFFECT::EFFECT_ALL];
	Entity* callback;

};