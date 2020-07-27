#pragma once
#ifndef __BULLET_ANIMATION_STATE__
#define __BULLET_ANIMATION_STATE__

enum BulletAnimationState
{
	BULLET_MOVE_RIGHT,
	BULLET_MOVE_LEFT,
	BULLET_CRASH_RIGHT,
	BULLET_CRASH_LEFT,
	NUM_OF_BULLET_ANIMATION_STATES
};

enum BulletType
{
	ENEMY_BULLET,
	PLAYER_BULLET,
	PLAYER_BULLET2,
	NUM_OF_BULLET_TYPE
};
#endif