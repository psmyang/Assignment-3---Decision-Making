#include "Bullet.h"
#include "TextureManager.h"

#define BULLETSPEED 10;
#define EBULLETSPEED 5;

Bullet::Bullet(float xPos, float yPos, /*bool bulletType*/BulletType bulletType, BulletAnimationState bulletState)
{
	//m_playerBullet = bulletType; // If we decide different enemies will have different colour/shape bullets then we will need to make an enum
	this->m_BulletType = bulletType;
	switch (m_BulletType)//if (m_playerBullet)
	{
	case PLAYER_BULLET:
	{
		std::cout << "Player bullet created " << std::endl;
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/beams.txt",
			"../Assets/sprites/beams.png",
			"player-bullet-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("player-bullet-sprite"));
		setType(P_BULLET);

		setWidth(20);
		setHeight(10);

		setAnimationState(bulletState);
		if (m_currentAnimationState == BULLET_MOVE_RIGHT)
		{
			m_speed = BULLETSPEED;
		}
		else if (m_currentAnimationState == BULLET_MOVE_LEFT)
		{
			m_speed = -BULLETSPEED;
		}
		break;
	}
	case ENEMY_BULLET://else
	{
		// Don't have enemy bullet sprite made yet but this would be same code as above but for enemy bullet sprite
		std::cout << "Enemy bullet created " << std::endl;
		TextureManager::Instance()->loadSpriteSheet(
			"../Assets/sprites/enemybullet.txt",
			"../Assets/sprites/enemybullet.png",
			"enemy-bullet-sprite"
		);

		setSpriteSheet(TextureManager::Instance()->getSpriteSheet("enemy-bullet-sprite"));
		setType(E_BULLET);

		setWidth(70);
		setHeight(41);


		setAnimationState(bulletState);
		if (m_currentAnimationState == BULLET_MOVE_RIGHT)
		{
			m_speed = EBULLETSPEED;
		}
		else if (m_currentAnimationState == BULLET_MOVE_LEFT)
		{
			m_speed = -EBULLETSPEED;
		}
		break;
	}
	default:
		break;
	}


	getTransform()->position = glm::vec2(xPos + 30, yPos + 27);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	m_buildAnimations();
}

Bullet::~Bullet() = default;

void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// Check if bullet is player's or enemy's to change appearance of bullet
	std::string sprite;

	//if (m_playerBullet) {
	//	sprite = "player-bullet-sprite";
	//}
	//else {
	//	sprite = "enemy-bullet-sprite";
	//}
	switch (m_BulletType)
	{
	case PLAYER_BULLET:
	{
		sprite = "player-bullet-sprite";
		break;
	}
	case ENEMY_BULLET:
	{
		sprite = "enemy-bullet-sprite";
		break;
	}
	default:
		break;
	}

	//draw the bullet according to animation state
	switch (m_currentAnimationState)
	{
	case BULLET_MOVE_RIGHT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("bullet-moving"),
			x, y, 0.12f, 0, 200, true);
		break;
	case BULLET_MOVE_LEFT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("bullet-moving"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case BULLET_CRASH_RIGHT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("bullet-colliding"),
			x, y, 0.25f, 0, 255, true);
		break;
	case BULLET_CRASH_LEFT:
		TextureManager::Instance()->playAnimation(sprite, getAnimation("bullet-colliding"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
}

void Bullet::update()
{
	getTransform()->position.x += m_speed;
}

void Bullet::clean()
{
}

void Bullet::setAnimationState(BulletAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Bullet::setPosition(int x, int y)
{
	getTransform()->position.x = x;
	getTransform()->position.y = y;
}

void Bullet::m_buildAnimations()
{
	Animation bMovingAnimation = Animation();

	bMovingAnimation.name = "bullet-moving";
	bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-0"));
	//bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-1"));
	//bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-2"));
	//bMovingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-moving-3"));

	setAnimation(bMovingAnimation);

	Animation bCollidingAnimation = Animation();

	bCollidingAnimation.name = "bullet-colliding";
	bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-0"));
	//bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-1"));
	//bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-2"));
	//bCollidingAnimation.frames.push_back(getSpriteSheet()->getFrame("bullet-colliding-3"));

	setAnimation(bCollidingAnimation);
}
