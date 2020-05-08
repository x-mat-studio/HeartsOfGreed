#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vector"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;

class Particle : public Entity
{
public:
	Particle();
	Particle(float life, SDL_Texture* texture, Animation animation, bool fade = false); //The emiter uses this constructor
	Particle(fMPoint& position, fMPoint& speed, fMPoint& acceleration, float angle, float angularSpeed, float life, SDL_Texture* texture, Animation animation, bool fade = false);
	Particle(float positionX, float positionY, float speedX, float speedY, float accelerationX, float accelerationY, float angle, float angularSpeed, float life, SDL_Texture* texture, Animation animation, bool fade = false);
	~Particle();

	//Getters and setters
	fMPoint GetPosition() const;
	fMPoint GetSpeed() const;
	fMPoint GetAcceleration() const;

	float GetAngle() const;
	float GetAngularSpeed() const;

	float GetLife() const;
	SDL_Texture* GetTexture() const;

	void SetPosition(fMPoint&);
	void SetSpeed(fMPoint&);
	void SetAcceleration(fMPoint&);

	void SetAngle(float);
	void SetAngularSpeed(float);

	//the actual useful functions
	bool Update(float dt);
	bool PostUpdate(float dt);

	bool Activate();
	void Reset(float x, float y, float speedX, float speedY, float accX, float accY, float angularSpeed);
	void Desactivate();

private:
	void Move(float dt);
	void Draw(float dt);

	void CheckLife(float dt);

private:
	SDL_Texture* texture;
	Animation animation;

	fMPoint position;
	fMPoint speed;
	fMPoint acceleration;

	float angle;
	float angularSpeed; //positive = right, negative = left

	float life;
	float originalLife;

	bool fade;
	bool active;
};


#endif //__PARTICLE_H__