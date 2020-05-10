#include "EasingFunctions.h"
#include "p2Defs.h"
#include <math.h>


float EaseFunctions::Ease(float currentTime, float initialPos, float finalPos, float duration)
{
	if (currentTime <= 0) return initialPos;
	if (currentTime >= duration) return initialPos + finalPos;
	else return finalPos * (currentTime / duration) + initialPos;

}


float EaseFunctions::EaseInQuad(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (currentTime /= duration) * currentTime + initialPos;
}


float EaseFunctions::EaseOutQuad(float currentTime, float initialPos, float finalPos, float duration)
{
	return -finalPos * (currentTime /= duration) * (currentTime - 2) + initialPos;
}


float EaseFunctions::EaseInOutQuad(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration / 2) < 1) return finalPos / 2 * currentTime * currentTime + initialPos;
	return -finalPos / 2 * ((--currentTime) * (currentTime - 2) - 1) + initialPos;
}


float EaseFunctions::EaseInCubic(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * pow((currentTime / duration), 3) + initialPos;
}


float EaseFunctions::EaseOutCubic(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (pow((currentTime / duration - 1), 3) + 1) + initialPos;
}


float EaseFunctions::EaseInOutCubic(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration / 2) < 1)return finalPos / 2 * pow(currentTime, 3) + initialPos;
	return finalPos / 2 * (pow(currentTime - 2, 3) + 2) + initialPos;
}


float EaseFunctions::EaseInQuart(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * pow(currentTime / duration, 4) + initialPos;
}


float EaseFunctions::EaseOutQuart(float currentTime, float initialPos, float finalPos, float duration)
{
	return -finalPos * (pow(currentTime / duration - 1, 4) - 1) + initialPos;
}


float EaseFunctions::EaseInOutQuart(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration / 2) < 1)return finalPos / 2 * pow(currentTime, 4) + initialPos;
	return -finalPos / 2 * (pow(currentTime - 2, 4) - 2) + initialPos;
}


float EaseFunctions::EaseInQuint(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * pow(currentTime / duration, 5) + initialPos;
}


float EaseFunctions::EaseOutQuint(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (pow(currentTime / duration - 1, 5) + 1) + initialPos;
}


float EaseFunctions::EaseInOutQuint(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration / 2) < 1) return finalPos / 2 * pow(currentTime, 5) + initialPos;
	return finalPos / 2 * (pow(currentTime - 2, 5) + 2) + initialPos;
}


float EaseFunctions::EaseInSine(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (1 - cos(currentTime / duration * (PI / 2))) + initialPos;
}


float EaseFunctions::EaseOutSine(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * sin(currentTime / duration * (PI / 2)) + initialPos;
}


float EaseFunctions::EaseInOutSine(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos / 2 * (1 - cos(PI * currentTime / duration)) + initialPos;
}


float EaseFunctions::EaseInExpo(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * pow(2, 10 * (currentTime / duration - 1)) + initialPos;
}


float EaseFunctions::EaseOutExpo(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (-pow(2, -10 * currentTime / duration) + 1) + initialPos;
}


float EaseFunctions::EaseInOutExpo(float currentTime, float initialPos, float finalPos, float duration)
{

	if ((currentTime /= duration / 2) < 1)	return finalPos / 2 * pow(2, 10 * (currentTime - 1)) + initialPos;
	return finalPos / 2 * (-pow(2, -10 * --currentTime) + 2) + initialPos;

}


float EaseFunctions::EaseInCirc(float currentTime, float initialPos, float finalPos, float duration)
{
	return finalPos * (1 - sqrt(1 - (currentTime /= duration) * currentTime)) + initialPos;
}


float EaseFunctions::EaseOutCirc(float currentTime, float initialPos, float finalPos, float duration)
{
	return  finalPos * sqrt(1 - (currentTime = currentTime / duration - 1) * currentTime) + initialPos;
}


float EaseFunctions::EaseInOutCirc(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration / 2) < 1) return finalPos / 2 * (1 - sqrt(1 - currentTime * currentTime)) + initialPos;
	return finalPos / 2 * (sqrt(1 - (currentTime -= 2) * currentTime) + 1) + initialPos;
}


float EaseFunctions::EaseOutBounce(float currentTime, float initialPos, float finalPos, float duration)
{
	if ((currentTime /= duration) < (1 / 2.75)) {
		return finalPos * (7.5625 * currentTime * currentTime) + initialPos;
	}
	else if (currentTime < (2 / 2.75)) {
		return finalPos * (7.5625 * (currentTime -= (1.5 / 2.75)) * currentTime + .75) + initialPos;
	}
	else if (currentTime < (2.5 / 2.75)) {
		return finalPos * (7.5625 * (currentTime -= (2.25 / 2.75)) * currentTime + .9375) + initialPos;
	}
	else {
		return finalPos * (7.5625 * (currentTime -= (2.625 / 2.75)) * currentTime + .984375) + initialPos;
	}

}


float EaseFunctions::EaseInBack(float currentTime, float initialPos, float finalPos, float duration)
{
	float s = 1.70158;
	return finalPos * (currentTime /= duration) * currentTime * ((s + 1) * currentTime - s) + initialPos;

}


float EaseFunctions::EaseOutBack(float currentTime, float initialPos, float finalPos, float duration)
{

	float s = 1.70158;
	return finalPos * ((currentTime = currentTime / duration - 1) * currentTime * ((s + 1) * currentTime + s) + 1) + initialPos;

}


float EaseFunctions::EaseInOutBack(float currentTime, float initialPos, float finalPos, float duration)
{

	float s = 1.70158;
	if ((currentTime /= duration / 2) < 1) return finalPos / 2 * (currentTime * currentTime * (((s *= (1.525)) + 1) * currentTime - s)) + initialPos;
	return finalPos / 2 * ((currentTime -= 2) * currentTime * (((s *= (1.525)) + 1) * currentTime + s) + 2) + initialPos;

}


float EaseFunctions::EaseOutElastic(float currentTime, float initialPos, float finalPos, float duration)
{

	if ((currentTime /= duration) == 1) {
		return initialPos + finalPos;
	}
	else {
		return (finalPos * pow(2, -10 * currentTime) * sin((currentTime * duration - ((duration * 0.3) / 4)) * (2 * 3.14) / (duration * 0.3)) + finalPos + initialPos);
	}

}


Easing::Easing() :initialPos(0.0f), finalPos(0.0f), currentTime(0.0f), duration(0.0f), lastRequestedPos(0.0f), type(EASING_TYPE::NONE), active(false)
{
}


Easing::Easing(EASING_TYPE type, float initialPos, float finalPos, float duration, float currentTime) : lastRequestedPos(initialPos)
{
	NewEasing(type, initialPos, finalPos, duration, currentTime);
}


Easing::~Easing()
{
	type = EASING_TYPE::NONE;
	initialPos = 0.0f;
	finalPos = 0.0f;
	duration = 0.0f;
	currentTime = 0.0f;
	lastRequestedPos = 0.0f;
	active = false;
}


void Easing::NewEasing(EASING_TYPE ntype, float ninitialPos, float nfinalPos, float nduration, float ncurrentTime)
{
	type = ntype;
	if (type == EASING_TYPE::NONE) type = EASING_TYPE::EASE;
	initialPos = ninitialPos;
	finalPos = nfinalPos;
	duration = MAX(nduration, 0.0f);

	currentTime = MAX(ncurrentTime, 0.0f);
	currentTime = MIN(currentTime, duration);

	lastRequestedPos = initialPos;

	active = true;
}


bool Easing::IsActive() const
{
	return active;
}


void Easing::Activate()
{
	active = true;
}

void Easing::Deactivate()
{
	active = false;
}


void Easing::ChangeActiveState(bool isActive)
{
	active = isActive;
}


EASING_TYPE Easing::GetType() const
{
	return type;
}


float Easing::UpdateEasingFromNewTime(float newCurrentTime)
{
	float ret;

	currentTime = MAX(newCurrentTime, 0.0f);
	currentTime = MIN(currentTime, duration);

	ret = ExecuteFunctionFromType(type, currentTime, initialPos, finalPos, duration);

	if (currentTime == duration)
	{
		active = false;
	}

	return ret;
}


float Easing::UpdateEasingAddingTime(float addTimeToCurrent)
{
	float ret;
	currentTime += Abs(addTimeToCurrent);

	currentTime = MAX(currentTime, 0.0f);
	currentTime = MIN(currentTime, duration);

	ret = ExecuteFunctionFromType(type, currentTime, initialPos, finalPos, duration);

	if (currentTime == duration)
	{
		active = false;
	}

	return ret;
}


float Easing::GetLastRequestedPos() const
{
	return lastRequestedPos;
}


float Easing::ExecuteFunctionFromType(EASING_TYPE type, float& currentTime, float& initialPos, float& finalPos, float& duration)
{
	switch (type)
	{
	case EASING_TYPE::EASE:
		lastRequestedPos = functions.Ease(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_QUAD:
		lastRequestedPos = functions.EaseInQuad(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_QUAD:
		lastRequestedPos = functions.EaseOutQuad(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_QUAD:
		lastRequestedPos = functions.EaseInOutQuad(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_CUBIC:
		lastRequestedPos = functions.EaseInCubic(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_CUBIC:
		lastRequestedPos = functions.EaseOutCubic(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_CUBIC:
		lastRequestedPos = functions.EaseInOutCubic(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_QUART:
		lastRequestedPos = functions.EaseInQuart(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_QUART:
		lastRequestedPos = functions.EaseOutQuart(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_QUART:
		lastRequestedPos = functions.EaseInOutQuart(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_QUINT:
		lastRequestedPos = functions.EaseInQuint(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_QUINT:
		lastRequestedPos = functions.EaseOutQuint(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_QUINT:
		lastRequestedPos = functions.EaseInOutQuint(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_SINE:
		lastRequestedPos = functions.EaseInSine(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_SINE:
		lastRequestedPos = functions.EaseOutSine(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_SINE:
		lastRequestedPos = functions.EaseInOutSine(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_EXPO:
		lastRequestedPos = functions.EaseInExpo(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_EXPO:
		lastRequestedPos = functions.EaseOutExpo(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_EXPO:
		lastRequestedPos = functions.EaseInOutExpo(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_CIRC:
		lastRequestedPos = functions.EaseInCirc(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_CIRC:
		lastRequestedPos = functions.EaseOutCirc(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_CIRC:
		lastRequestedPos = functions.EaseInOutCirc(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_BOUNCE:
		lastRequestedPos = functions.EaseOutBounce(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_BACK:
		lastRequestedPos = functions.EaseInBack(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_BACK:
		lastRequestedPos = functions.EaseOutBack(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_IN_OUT_BACK:
		lastRequestedPos = functions.EaseInOutBack(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::EASE_OUT_ELASTIC:
		lastRequestedPos = functions.EaseOutElastic(currentTime, initialPos, finalPos, duration);
		break;
	case EASING_TYPE::NONE:
		break;
	}

	active = true;

	return lastRequestedPos;
}
