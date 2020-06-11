#include "EasingFunctions.h"
#include "p2Defs.h"
#include <math.h>


double EaseFunctions::Ease(double currentTime, double initialPos, double finalPos, double duration)
{
	if (currentTime <= 0) return initialPos;
	if (currentTime >= duration) return finalPos;
	else return (finalPos - initialPos) * (currentTime / duration) + initialPos;

}


double EaseFunctions::EaseInQuad(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (currentTime /= duration) * currentTime + initialPos;
}


double EaseFunctions::EaseOutQuad(double currentTime, double initialPos, double finalPos, double duration)
{
	return -(finalPos - initialPos) * (currentTime /= duration) * (currentTime - 2.0) + initialPos;

}


double EaseFunctions::EaseInOutQuad(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration / 2) < 1) return (finalPos - initialPos) / 2 * currentTime * currentTime + initialPos;
	return -(finalPos - initialPos) / 2 * ((--currentTime) * (currentTime - 2) - 1) + initialPos;
}


double EaseFunctions::EaseInCubic(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * pow((currentTime / duration), 3) + initialPos;
}


double EaseFunctions::EaseOutCubic(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (pow(((currentTime / duration) - 1.0), 3) + 1) + initialPos;
}


double EaseFunctions::EaseInOutCubic(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration / 2) < 1)return (finalPos - initialPos) / 2 * pow(currentTime, 3) + initialPos;
	return (finalPos - initialPos) / 2 * (pow(currentTime - 2.0, 3) + 2) + initialPos;
}


double EaseFunctions::EaseInQuart(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * pow(currentTime / duration, 4) + initialPos;
}


double EaseFunctions::EaseOutQuart(double currentTime, double initialPos, double finalPos, double duration)
{
	return -(finalPos - initialPos) * (pow((currentTime / duration) - 1.0, 4) - 1) + initialPos;
}


double EaseFunctions::EaseInOutQuart(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration / 2) < 1)return (finalPos - initialPos) / 2 * pow(currentTime, 4) + initialPos;
	return -(finalPos - initialPos) / 2 * (pow(currentTime - 2.0, 4) - 2) + initialPos;
}


double EaseFunctions::EaseInQuint(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * pow(currentTime / duration, 5) + initialPos;
}


double EaseFunctions::EaseOutQuint(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (pow((currentTime / duration) - 1.0, 5) + 1) + initialPos;
}


double EaseFunctions::EaseInOutQuint(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration / 2) < 1) return (finalPos - initialPos) / 2 * pow(currentTime, 5) + initialPos;
	return (finalPos - initialPos) / 2 * (pow(currentTime - 2.0, 5) + 2) + initialPos;
}


double EaseFunctions::EaseInSine(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (1 - cos(currentTime / duration * (PI / 2))) + initialPos;
}


double EaseFunctions::EaseOutSine(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * sin(currentTime / duration * (PI / 2)) + initialPos;
}


double EaseFunctions::EaseInOutSine(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) / 2 * (1 - cos(PI * currentTime / duration)) + initialPos;
}


double EaseFunctions::EaseInExpo(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * pow(2, 10 * ((currentTime / duration) - 1.0)) + initialPos;
}


double EaseFunctions::EaseOutExpo(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (-pow(2, -10 * currentTime / duration) + 1) + initialPos;
}


double EaseFunctions::EaseInOutExpo(double currentTime, double initialPos, double finalPos, double duration)
{

	if ((currentTime /= duration / 2.0) < 1.0)	return (finalPos - initialPos) / 2.0 * pow(2.0, 10.0 * (currentTime - 1.0)) + initialPos;
	return (finalPos - initialPos) / 2 * (-pow(2.0, -10.0 * --currentTime) + 2.0) + initialPos;

}


double EaseFunctions::EaseInCirc(double currentTime, double initialPos, double finalPos, double duration)
{
	return (finalPos - initialPos) * (1.0 - sqrt(1.0 - (currentTime /= duration) * currentTime)) + initialPos;
}


double EaseFunctions::EaseOutCirc(double currentTime, double initialPos, double finalPos, double duration)
{
	return  (finalPos - initialPos) * sqrt(1.0 - (currentTime = currentTime / duration - 1.0) * currentTime) + initialPos;
}


double EaseFunctions::EaseInOutCirc(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration / 2.0) < 1.0) return (finalPos - initialPos) / 2.0 * (1 - sqrt(1.0 - currentTime * currentTime)) + initialPos;
	return (finalPos - initialPos) / 2.0 * (sqrt(1.0 - (currentTime = currentTime - 2.0) * currentTime) + 1.0) + initialPos;
}


double EaseFunctions::EaseOutBounce(double currentTime, double initialPos, double finalPos, double duration)
{
	if ((currentTime /= duration) < (1.0 / 2.75)) {
		return (finalPos - initialPos) * (7.5625 * currentTime * currentTime) + initialPos;
	}
	else if (currentTime < (2.0 / 2.75)) {
		return (finalPos - initialPos) * (7.5625 * (currentTime = currentTime - (1.5 / 2.75)) * currentTime + .75) + initialPos;
	}
	else if (currentTime < (2.5 / 2.75)) {
		return (finalPos - initialPos) * (7.5625 * (currentTime = currentTime - (2.25 / 2.75)) * currentTime + .9375) + initialPos;
	}
	else {
		return (finalPos - initialPos) * (7.5625 * (currentTime = currentTime - (2.625 / 2.75)) * currentTime + .984375) + initialPos;
	}

}


double EaseFunctions::EaseInBack(double currentTime, double initialPos, double finalPos, double duration)
{
	double s = 1.70158;
	return (finalPos - initialPos) * (currentTime /= duration) * currentTime * ((s + 1) * currentTime - s) + initialPos;

}


double EaseFunctions::EaseOutBack(double currentTime, double initialPos, double finalPos, double duration)
{

	double s = 1.70158;
	return (finalPos - initialPos) * ((currentTime = currentTime / duration - 1) * currentTime * ((s + 1) * currentTime + s) + 1) + initialPos;

}


double EaseFunctions::EaseInOutBack(double currentTime, double initialPos, double finalPos, double duration)
{

	double s = 1.70158;
	if ((currentTime /= duration / 2) < 1) return (finalPos - initialPos) / 2 * (currentTime * currentTime * (((s *= (1.525)) + 1) * currentTime - s)) + initialPos;
	return (finalPos - initialPos) / 2 * ((currentTime -= 2) * currentTime * (((s *= (1.525)) + 1) * currentTime + s) + 2) + initialPos;

}


double EaseFunctions::EaseOutElastic(double currentTime, double initialPos, double finalPos, double duration)
{

	if ((currentTime /= duration) == 1) {
		return finalPos;
	}
	else {
		return ((finalPos - initialPos) * pow(2.0, -10.0 * currentTime) * sin((currentTime * duration - ((duration * 0.3) / 4.0)) * (2.0 * 3.14) / (duration * 0.3)) + (finalPos - initialPos) + initialPos);
	}

}


Easing::Easing() :initialPos(0.0f), finalPos(0.0f), currentTime(0.0f), duration(0.0f), lastRequestedPos(0.0f), type(EASING_TYPE::NONE), active(false)
{
}


Easing::Easing(EASING_TYPE type, double initialPos, double finalPos, double duration, double currentTime) : lastRequestedPos(initialPos)
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


void Easing::NewEasing(EASING_TYPE ntype, double ninitialPos, double nfinalPos, double nduration, double ncurrentTime)
{
	type = ntype;
	if (type == EASING_TYPE::NONE) type = EASING_TYPE::EASE;
	initialPos = ninitialPos;
	finalPos = nfinalPos;
	duration = MAX(nduration, 0.0f);

	currentTime = MAX(ncurrentTime, 0.0f);
	currentTime = MIN(currentTime, duration);

	if (currentTime != 0.0)
	{
		lastRequestedPos = UpdateEasingFromNewTime(ncurrentTime);
	}
	else
	{
		lastRequestedPos = initialPos;
	}

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


double Easing::GetInitialPos() const
{
	return initialPos;
}

double Easing::GetFinalPos() const
{
	return finalPos;
}

double Easing::GetDuration() const
{
	return duration;
}

double Easing::GetCurrentTime() const
{
	return currentTime;
}


double Easing::UpdateEasingFromNewTime(double newCurrentTime)
{
	double ret;

	currentTime = MAX(newCurrentTime, 0.0f);
	currentTime = MIN(currentTime, duration);

	ret = ExecuteFunctionFromType(type, currentTime, initialPos, finalPos, duration);

	if (currentTime == duration)
	{
		active = false;
	}

	return ret;
}


double Easing::UpdateEasingAddingTime(double addTimeToCurrent)
{
	double ret;
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


double Easing::GetLastRequestedPos() const
{
	return lastRequestedPos;
}


double Easing::ExecuteFunctionFromType(EASING_TYPE type, double currentTime, double initialPos, double finalPos, double duration)
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
