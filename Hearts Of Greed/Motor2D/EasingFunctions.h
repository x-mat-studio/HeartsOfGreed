#ifndef __EASING_FUNCTIONS_H__
#define __EASING_FUNCTIONS_H__


struct EaseFunctions {

	float Ease(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInQuad(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutQuad(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutQuad(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInCubic(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutCubic(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutCubic(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInQuart(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutQuart(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutQuart(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInQuint(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutQuint(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutQuint(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInSine(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutSine(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutSine(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInExpo(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutExpo(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutExpo(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInCirc(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutCirc(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutCirc(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseOutBounce(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseInBack(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseOutBack(float& currentTime, float& initialPos, float& finalPos, float& duration);
	float EaseInOutBack(float& currentTime, float& initialPos, float& finalPos, float& duration);

	float EaseOutElastic(float& currentTime, float& initialPos, float& finalPos, float& duration);

};


#endif // !__EASING_FUNCTIONS_H__

