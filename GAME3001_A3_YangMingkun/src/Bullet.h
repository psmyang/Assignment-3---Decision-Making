#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "Sprite.h"
#include "BulletAnimationState.h"

class Bullet : public Sprite
{
public:
	Bullet(float xPos, float yPos, /*bool bulletType*/ BulletType bulletType, BulletAnimationState bulletState);
	~Bullet();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// Setters
	void setAnimationState(BulletAnimationState new_state);
	void setPosition(int x, int y);
private:
	int m_speed;
	bool m_active;
	//bool m_playerBullet;
	BulletType m_BulletType;

	SDL_Rect m_rDst;

	void m_buildAnimations();
	BulletAnimationState m_currentAnimationState;


};
#endif

