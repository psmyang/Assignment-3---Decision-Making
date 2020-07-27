#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "DebugKeys.h"
#include "Obstacle.h"
#include "PathNode.h"
#include "SoundManager.h"
#include "Bullet.h"
#include "Scoreboard.h"
#include "Background.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void PlayerShoot();

private:
	// PRIVATE FUNCTIONS
	void m_buildGrid();

	void m_displayGrid();

	void m_displayGridLOS();

	void m_setGridLOS();

	// MEMBER VARIABLES
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	bool m_playerFacingRight;
	bool m_bPlayerHasLOS;

	Obstacle* m_pObstacle;

	bool m_bDebugMode;
	bool m_bPatrolMode;

	bool m_bDebugKeys[NUM_OF_DEBUG_KEYS];

	std::vector<PathNode*> m_pGrid;
	
	std::vector<PathNode*> m_pPatrolPath;
	void m_buildClockwisePatrolPath();
	void m_displayPatrolPath();
	int m_targetPathNodeIndex;
	PathNode* m_pTargetPathNode;
	void m_movePlaneToTargetNode();

	std::vector<Bullet*> m_pPlayerBulletVec;
	std::vector<Bullet*> m_pEnemyBulletVec;

	ScoreBoard* m_pScoreBoard;
	ScoreBoard* Health;
	int PlayerHealth;

	Background* m_pBackground;

};

#endif /* defined (__PLAY_SCENE__) */