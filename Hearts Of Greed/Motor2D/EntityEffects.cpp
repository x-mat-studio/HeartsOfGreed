#include "EntityEffects.h"
#include "DynamicEntity.h"

EntityEffects::EntityEffects() : effects{}, callback(nullptr)
{}


EntityEffects::~EntityEffects()
{
	callback = nullptr;
}


void EntityEffects::AddNewEffect(SKILL_EFFECT& effect, float time, float severity)
{
	effects[effect].time = time;
	effects[effect].severity = severity;

	StartEffect(effect);
}


void EntityEffects::UpdateEffects(float dt)
{
	if (callback == nullptr)
		return;

	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (effects[i].time > 0.f)
		{
			UpdateEffect(SKILL_EFFECT(i));
			effects[i].time -= dt;
		}
	}

}


void EntityEffects::DeleteEffect(SKILL_EFFECT& effect)
{
	effects[effect].time = 0.f;
	effects[effect].severity = 0.f;
	effects[effect].statTaken = 0.f;
}


void EntityEffects::DeleteAllEffects()
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		effects[i].time = 0.f;
		effects[i].severity = 0.f;
		effects[i].statTaken = 0.f;
	}
}


void EntityEffects::StartEffect(SKILL_EFFECT effect)
{
	switch (effect)
	{
	case SKILL_EFFECT::SLOWDOWN:

		break;


	case SKILL_EFFECT::BLOOD_LOSS:

		float currHp = callback->GetCurrentHP();

		effects[effect].statTaken = currHp / effects[effect].severity / effects[effect].time / 60;

		break;
	}
}


void EntityEffects::UpdateEffect(SKILL_EFFECT effect)
{
	switch (effect)
	{
	case SKILL_EFFECT::SLOWDOWN:
	
		if (callback->dynamic == true)
		{
			DynamicEntity* dynamic = (DynamicEntity*)callback;

			int unitSpeed = dynamic->GetSpeed();

			int subtractSpeed = effects[effect].severity * unitSpeed;

			dynamic->SetSpeed(unitSpeed - subtractSpeed);
		}
		break;


	case SKILL_EFFECT::BLOOD_LOSS:

		callback->RecieveDamage(effects[effect].statTaken);

		break;

	}


}


SkillEffect::SkillEffect() : time(0.f), severity(0.f), statTaken(0)
{}
