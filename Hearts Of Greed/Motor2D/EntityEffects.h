#include "Entity.h"

struct SkillEffect
{
	SkillEffect();

	float time;
	float severity;
	float statTaken;

	bool active;
};

struct EntityEffects
{
	EntityEffects();
	~EntityEffects();

public:
	void UpdateEffects(float dt);

	void DeleteEffect(SKILL_EFFECT& effect);
	void DeleteAllEffects();

	void AddNewEffect(SKILL_EFFECT effect, float time, float severity = 1.0f);

	void SetCallBack(Entity*);

	bool IsStuned();

private:

	void StartEffect(SKILL_EFFECT effect);
	void EndEffect(SKILL_EFFECT effect);

	void UpdateEffect(SKILL_EFFECT effect);

	bool IsAnyEffectActive();

private:

	SkillEffect effects[SKILL_EFFECT::EFFECT_ALL];
	Entity* callback;

};