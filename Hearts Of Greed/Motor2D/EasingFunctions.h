#ifndef __EASING_FUNCTIONS_H__
#define __EASING_FUNCTIONS_H__

enum class EASING_TYPE
{
	EASE,

	EASE_IN_QUAD,
	EASE_OUT_QUAD,
	EASE_IN_OUT_QUAD,

	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_IN_OUT_CUBIC,

	EASE_IN_QUART,
	EASE_OUT_QUART,
	EASE_IN_OUT_QUART,

	EASE_IN_QUINT,
	EASE_OUT_QUINT,
	EASE_IN_OUT_QUINT,

	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_IN_OUT_SINE,

	EASE_IN_EXPO,
	EASE_OUT_EXPO,
	EASE_IN_OUT_EXPO,

	EASE_IN_CIRC,
	EASE_OUT_CIRC,
	EASE_IN_OUT_CIRC,

	EASE_OUT_BOUNCE,

	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_OUT_BACK,

	EASE_OUT_ELASTIC,

	NONE
};


struct EaseFunctions {

	float Ease(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInQuad(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutQuad(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutQuad(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInCubic(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutCubic(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutCubic(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInQuart(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutQuart(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutQuart(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInQuint(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutQuint(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutQuint(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInSine(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutSine(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutSine(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInExpo(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutExpo(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutExpo(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInCirc(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutCirc(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutCirc(float currentTime, float initialPos, float finalPos, float duration);

	float EaseOutBounce(float currentTime, float initialPos, float finalPos, float duration);

	float EaseInBack(float currentTime, float initialPos, float finalPos, float duration);
	float EaseOutBack(float currentTime, float initialPos, float finalPos, float duration);
	float EaseInOutBack(float currentTime, float initialPos, float finalPos, float duration);

	float EaseOutElastic(float currentTime, float initialPos, float finalPos, float duration);

};


class Easing
{
public:
	Easing();
	Easing(EASING_TYPE type, float initialPos, float finalPos, float duration,float currentTime=0.0f);
	~Easing();

	//creates a new easing and sets it to active
	void NewEasing(EASING_TYPE type, float initialPos, float finalPos, float duration, float currentTime = 0.0f);
	
	bool IsActive()const;
	void Activate();
	void Deactivate();
	void ChangeActiveState(bool isActive);

	EASING_TYPE GetType()const;

	//outputs the current position based on an input time (it also changes the state to active if it isn't already)
	float UpdateEasingFromNewTime(float newCurrentTime);
	//outputs the current position based on an input time (it also changes the state to active if it isn't already)
	float UpdateEasingAddingTime(float addTimeToCurrent);

	//returns the last output that was given when the function last updated without the need to execute the function again
	float GetLastRequestedPos()const;

private:
	float ExecuteFunctionFromType(EASING_TYPE type, float& currentTime, float& initialPos, float& finalPos, float& duration);


private:
	float initialPos;
	float finalPos;
	float currentTime;
	float duration;

	float lastRequestedPos;

	EASING_TYPE type;
	EaseFunctions functions;

	bool active;
};

#endif // !__EASING_FUNCTIONS_H__

