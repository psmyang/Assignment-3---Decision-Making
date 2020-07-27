#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include <algorithm>


PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	if(m_bDebugMode)
	{
		auto LOSColour = (!m_bPlayerHasLOS) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		Util::DrawLine(m_pPlayer->getTransform()->position, m_pPlaneSprite->getTransform()->position, LOSColour);

		Util::DrawRect(m_pPlayer->getTransform()->position - glm::vec2(m_pPlayer->getWidth() * 0.5f, m_pPlayer->getHeight() *0.5f),
			m_pPlayer->getWidth(), m_pPlayer->getHeight());

		Util::DrawRect(m_pPlaneSprite->getTransform()->position - glm::vec2(m_pPlaneSprite->getWidth() * 0.5f, m_pPlaneSprite->getHeight() * 0.5f),
			m_pPlaneSprite->getWidth(), m_pPlaneSprite->getHeight());

		Util::DrawRect(m_pObstacle->getTransform()->position - glm::vec2(m_pObstacle->getWidth() * 0.5f, m_pObstacle->getHeight() * 0.5f),
			m_pObstacle->getWidth(), m_pObstacle->getHeight());

		m_displayGrid();

		//m_displayGridLOS();
	}
}

void PlayScene::update()
{
	updateDisplayList();

	m_bPlayerHasLOS = CollisionManager::LOSCheck(m_pPlayer, m_pPlaneSprite, m_pObstacle);

	CollisionManager::AABBCheck(m_pPlayer, m_pPlaneSprite);

	CollisionManager::AABBCheck(m_pPlayer, m_pObstacle);

	m_setGridLOS();

	m_movePlaneToTargetNode();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;

				m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
				m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
				m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}
	

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("walk");
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("walk");
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_UP);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("walk");
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_DOWN);
			m_playerFacingRight = true;

			m_pPlayer->getRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
			m_pPlayer->getTransform()->position += m_pPlayer->getRigidBody()->velocity;
			m_pPlayer->getRigidBody()->velocity *= m_pPlayer->getRigidBody()->velocity * 0.9f;
			SoundManager::Instance().playSound("walk");
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
		

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && !m_pPlayer->isShooting())
		{
			m_pPlayer->SetShooting(true);
			PlayerShoot();
		}
		else if (EventManager::Instance().isKeyUp(SDL_SCANCODE_SPACE) && m_pPlayer->isShooting())
		{
			m_pPlayer->SetShooting(false);
		}

		
	}

	// H KEY Section
	
	if(!m_bDebugKeys[H_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
		{
			// toggles Debug Mode
			m_bDebugMode = !m_bDebugMode;

			m_bDebugKeys[H_KEY] = true;

			if (m_bDebugMode)
			{
				std::cout << "DEBUG Mode On" << std::endl;
			}
			else
			{
				std::cout << "DEBUG Mode Off" << std::endl;
			}
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_H))
	{
		m_bDebugKeys[H_KEY] = false;
	}

	// K KEY Section
	
	if (!m_bDebugKeys[K_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
		{
			std::cout << "DEBUG: Enemies taking damage!" << std::endl;

			m_bDebugKeys[K_KEY] = true;
		}
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_K))
	{
		m_bDebugKeys[K_KEY] = false;
	}

	// P KEY Section

	if (!m_bDebugKeys[P_KEY])
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
		{
			// toggles Patrol Mode
			m_bPatrolMode = !m_bPatrolMode;

			m_bDebugKeys[P_KEY] = true;

			if (m_bPatrolMode)
			{
				std::cout << "DEBUG: Patrol Mode On" << std::endl;
			}
			else
			{
				std::cout << "DEBUG: Patrol Mode Off" << std::endl;
			}
			SoundManager::Instance().playSound("shot");
		}
		
	}

	if (EventManager::Instance().isKeyUp(SDL_SCANCODE_P))
	{
		m_bDebugKeys[P_KEY] = false;
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::m_buildGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto pathNode = new PathNode();
			pathNode->getTransform()->position = glm::vec2(pathNode->getWidth() * col + Config::TILE_SIZE * 0.5, pathNode->getHeight() * row + Config::TILE_SIZE * 0.5);
			m_pGrid.push_back(pathNode);
		}
	}

	std::cout << "Number of Nodes: " << m_pGrid.size() << std::endl;
}

void PlayScene::m_displayGrid()
{
	// Logic to add PathNodes to the scene
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto colour = (m_pGrid[row * Config::COL_NUM + col]->getLOS()) ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position - glm::vec2(m_pGrid[row * Config::COL_NUM + col]->getWidth() * 0.5f, m_pGrid[row * Config::COL_NUM + col]->getHeight() * 0.5f),
				Config::TILE_SIZE, Config::TILE_SIZE);

			Util::DrawRect(m_pGrid[row * Config::COL_NUM + col]->getTransform()->position, 5, 5, colour);
		}
	}
}

void PlayScene::m_displayGridLOS()
{
	for (auto node : m_pGrid)
	{
		if(!node->getLOS())
		{
			auto colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

			Util::DrawLine(node->getTransform()->position, m_pPlayer->getTransform()->position, colour);
		}
		
	}
}

void PlayScene::m_setGridLOS()
{
	for (auto node : m_pGrid)
	{
		node->setLOS(CollisionManager::LOSCheck(node, m_pPlayer, m_pObstacle));
	}
}

void PlayScene::m_buildClockwisePatrolPath()
{
	// moving right
	for (auto i = 0; i < Config::COL_NUM; i++)
	{
		m_pPatrolPath.push_back(m_pGrid[i]);
	}
	
	// moving down
	for (auto i = 0; i < Config::ROW_NUM; i++)
	{
		m_pPatrolPath.push_back(m_pGrid[i * Config::COL_NUM + Config::COL_NUM - 1]);
	}

	// moving left
	for (auto i = 0; i < Config::COL_NUM; i++)
	{
		m_pPatrolPath.push_back(m_pGrid[Config::COL_NUM * Config::ROW_NUM - 1 - i]);
	}

	// moving up
	for (auto i = Config::ROW_NUM - 2; i > 0; i--)
	{
		m_pPatrolPath.push_back(m_pGrid[i * Config::COL_NUM]);
	}
}

void PlayScene::m_displayPatrolPath()
{
	for (auto node : m_pPatrolPath)
	{
		std::cout << "(" << node->getTransform()->position.x << ", " << node->getTransform()->position.y << ")" << std::endl;
	}
}

void PlayScene::m_movePlaneToTargetNode()
{
	if (m_bPatrolMode)
	{
		m_pTargetPathNode = m_pPatrolPath[m_targetPathNodeIndex];
		auto targetVector = Util::normalize(m_pTargetPathNode->getTransform()->position - m_pPlaneSprite->getTransform()->position);

		// rotate PlaneSprite's Texture according to direction of travel
		if (targetVector.x == 1)
		{
			m_pPlaneSprite->setAngle(90.0f);
		}
		else if (targetVector.x == -1)
		{
			m_pPlaneSprite->setAngle(-90.0f);
		}

		if (targetVector.y == 1)
		{
			m_pPlaneSprite->setAngle(180.0f);
		}
		else if (targetVector.y == -1)
		{
			m_pPlaneSprite->setAngle(0.0f);
		}

		m_pPlaneSprite->getRigidBody()->velocity = targetVector;
		m_pPlaneSprite->getTransform()->position += m_pPlaneSprite->getRigidBody()->velocity * m_pPlaneSprite->getRigidBody()->maxSpeed;
		if (m_pPlaneSprite->getTransform()->position == m_pTargetPathNode->getTransform()->position)
		{
			m_targetPathNodeIndex++;
			if (m_targetPathNodeIndex > m_pPatrolPath.size() - 1)
			{
				m_targetPathNodeIndex = 0;
			}
		}
	}
}

void PlayScene::start()
{
	// Background
	m_pBackground = new Background("../Assets/backgrounds/startscene.png", "startscene-background", BACKGROUND, glm::vec2(12, 12), true);
	addChild(m_pBackground);

	m_bPlayerHasLOS = false;

	m_buildGrid();

	m_buildClockwisePatrolPath();
	//m_displayPatrolPath();
	m_targetPathNodeIndex = 1;
	

	m_bDebugMode = false;
	m_bPatrolMode = false;
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	m_pPlaneSprite->getTransform()->position = m_pPatrolPath[0]->getTransform()->position;
	m_pPlaneSprite->getRigidBody()->maxSpeed = 5.0f;
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(600.0f, 500.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Obstacle Texture
	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pObstacle);
	
	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(200.0f, 200.0f);
	addChild(m_pObstacle);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(200.0f, 400.0f);
	addChild(m_pObstacle);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(600.0f, 150.0f);
	addChild(m_pObstacle);

	//Health
	PlayerHealth = 100;
	const SDL_Color red = { 255, 0, 0, 255 };
	Health = new ScoreBoard("Health:" + std::to_string(PlayerHealth), "Playbill", 60, red, glm::vec2(100.0f, 550.0f));
	addChild(Health);

	//Score Board
	const SDL_Color red1 = { 255, 0, 0, 255 };
	m_pScoreBoard = new ScoreBoard("Score:", "Playbill", 60, red1, glm::vec2(600.0f, 550.0f));
	m_pScoreBoard->setParent(this);
	addChild(m_pScoreBoard);

	SoundManager::Instance().playMusic("startScreen");
}

void PlayScene::PlayerShoot()
{
	float x;
	float y = m_pPlayer->getTransform()->position.y;

	BulletAnimationState bState;

	if (m_playerFacingRight)
	{
		bState = BULLET_MOVE_RIGHT;
		x = m_pPlayer->getTransform()->position.x + 20;
	}
	else
	{
		bState = BULLET_MOVE_LEFT;
		x = m_pPlayer->getTransform()->position.x - 10;
	}

	m_pPlayerBulletVec.push_back(new Bullet(x, y, /*true*/PLAYER_BULLET, bState));
	addChild(m_pPlayerBulletVec[m_pPlayerBulletVec.size() - 1]);

	SoundManager::Instance().playSound("shot");
}
