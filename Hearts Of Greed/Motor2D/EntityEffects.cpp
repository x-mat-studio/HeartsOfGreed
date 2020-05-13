#include "EntityEffects.h"
#include "DynamicEntity.h"

EntityEffects::EntityEffects() : effects{}, callback(nullptr)
{}

void EntityEffects::AddNewEffect(SKILL_EFFECT& effect, float time, float severity)
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (SKILL_EFFECT(i) == effect)
		{
			effects[i].time = time;
		}
	}

}

void EntityEffects::UpdateEffects()
{
	if (callback == nullptr)
		return;

	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (effects[i].time > 0.f)
		{
			UpdateEffect(SKILL_EFFECT(i));
		}
	}

}

void EntityEffects::DeleteEffect(SKILL_EFFECT& effect)
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (SKILL_EFFECT(i) == effect)
		{
			effects[i].time = 0.f;
		}
	}
}

void EntityEffects::DeleteAllEffects()
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		effects[i].time = 0.f;
	}
}

void EntityEffects::UpdateEffect(SKILL_EFFECT effect)
{
	switch (effect)
	{
	case SKILL_EFFECT::SLOWDOWN:
	{
		if (callback->dynamic == true)
		{
			DynamicEntity* dynamic = (DynamicEntity*)callback;

			int unitSpeed = dynamic->GetSpeed();

			int subtractSpeed = effects[effect].severity * unitSpeed;

			dynamic->SetSpeed(unitSpeed - subtractSpeed);
		}
	}
		break;
	}


}

SkillEffect::SkillEffect() : time(0.f), severity(0.f), statTaken(0)
{}
