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

	double Ease(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInQuad(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutQuad(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutQuad(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInCubic(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutCubic(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutCubic(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInQuart(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutQuart(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutQuart(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInQuint(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutQuint(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutQuint(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInSine(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutSine(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutSine(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInExpo(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutExpo(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutExpo(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInCirc(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutCirc(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutCirc(double currentTime, double initialPos, double finalPos, double duration);

	double EaseOutBounce(double currentTime, double initialPos, double finalPos, double duration);

	double EaseInBack(double currentTime, double initialPos, double finalPos, double duration);
	double EaseOutBack(double currentTime, double initialPos, double finalPos, double duration);
	double EaseInOutBack(double currentTime, double initialPos, double finalPos, double duration);

	double EaseOutElastic(double currentTime, double initialPos, double finalPos, double duration);

};


class Easing
{
public:
	Easing();
	Easing(EASING_TYPE type, double initialPos, double finalPos, double duration,double currentTime=0.0f);
	~Easing();

	//creates a new easing and sets it to active
	void NewEasing(EASING_TYPE type, double initialPos, double finalPos, double duration, double currentTime = 0.0f);
	
	bool IsActive()const;
	void Activate();
	void Deactivate();
	void ChangeActiveState(bool isActive);

	EASING_TYPE GetType()const;
	double GetInitialPos()const;
	double GetFinalPos()const;
	double GetDuration()const;
	double GetCurrentTime()const;

	//outputs the current position based on an input time (it also changes the state to active if it isn't already)
	double UpdateEasingFromNewTime(double newCurrentTime);
	//outputs the current position based on an input time (it also changes the state to active if it isn't already)
	double UpdateEasingAddingTime(double addTimeToCurrent);

	//returns the last output that was given when the function last updated without the need to execute the function again
	double GetLastRequestedPos()const;

private:
	double ExecuteFunctionFromType(EASING_TYPE type, double currentTime, double initialPos, double finalPos, double duration);


private:
	double initialPos;
	double finalPos;
	double currentTime;
	double duration;

	double lastRequestedPos;

	EASING_TYPE type;
	EaseFunctions functions;

	bool active;
};

#endif // !__EASING_FUNCTIONS_H__

