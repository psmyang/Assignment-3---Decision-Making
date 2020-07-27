#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;



	// setters
	void setAnimationState(PlayerAnimationState new_state);

	bool isShooting();
	void SetShooting(bool s);

private:
	void m_buildAnimations();

	PlayerAnimationState m_currentAnimationState;

	bool m_shooting;
};

#endif /* defined (__PLAYER__) */