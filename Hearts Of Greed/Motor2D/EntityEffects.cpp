#include "EntityEffects.h"
#include "DynamicEntity.h"

EntityEffects::EntityEffects() : effects{}, callback(nullptr)
{}


EntityEffects::~EntityEffects()
{
	callback = nullptr;
}


void EntityEffects::AddNewEffect(SKILL_EFFECT effect, float time, float severity)
{
	if (effect == SKILL_EFFECT::BLOOD_LOSS)
	{
		if (IsAnyEffectActive() == false)
			return;
	}


	effects[effect].time = time;
	effects[effect].severity = severity;

	effects[effect].active = true;

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
		else if (effects[i].active == true)
			EndEffect(SKILL_EFFECT(i));
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

		if (callback->dynamic == true)
		{
			DynamicEntity* dynamic = (DynamicEntity*)callback;

			int unitSpeed = effects[effect].statTaken = dynamic->GetSpeed();

			int subtractSpeed = effects[effect].severity * unitSpeed;

			dynamic->SetSpeed(unitSpeed - subtractSpeed);
		}

		break;


	case SKILL_EFFECT::BLOOD_LOSS:
	{

		float maxHp = callback->GetMaxHP();
		effects[effect].statTaken = maxHp / effects[effect].severity / effects[effect].time / 60;

	}
	break;

	case SKILL_EFFECT::KNOCKDOWN:
	{
		if (callback->dynamic == true)
		{
			DynamicEntity* dynamic = (DynamicEntity*)callback;

			int unitSpeed = effects[effect].statTaken = dynamic->GetSpeed();

			int subtractSpeed = unitSpeed;

			dynamic->SetSpeed(unitSpeed - subtractSpeed);
		}
	}
	break;
	}
}


void EntityEffects::EndEffect(SKILL_EFFECT effect)
{
	DynamicEntity* dynamic = nullptr;

	switch (effect)
	{
	case SKILL_EFFECT::SLOWDOWN:
	case SKILL_EFFECT::KNOCKDOWN:

		dynamic = (DynamicEntity*)callback;
		dynamic->SetSpeed(effects[effect].statTaken);

		break;


	case SKILL_EFFECT::BLOOD_LOSS:
		break;
	}

	effects[effect].active = false;
}


void EntityEffects::UpdateEffect(SKILL_EFFECT effect)
{
	switch (effect)
	{
	case SKILL_EFFECT::SLOWDOWN:


		break;


	case SKILL_EFFECT::BLOOD_LOSS:

		callback->RecieveDamage(effects[effect].statTaken);

		break;

	}
}

bool EntityEffects::IsAnyEffectActive()
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (effects[i].time > 0.f)
		{
			return true;
		}
	}

	return false;
}


void EntityEffects::SetCallBack(Entity* entity)
{
	callback = entity;
}

bool EntityEffects::IsStuned()
{
	for (int i = 0; i < SKILL_EFFECT::EFFECT_ALL; i++)
	{
		if (effects[i].time > 0.f && SKILL_EFFECT(i) == SKILL_EFFECT::KNOCKDOWN)
		{
			return true;
		}
	}

	return false;
}


SkillEffect::SkillEffect() : time(0.f), severity(0.f), statTaken(0), active(false)
{}


