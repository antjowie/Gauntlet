#include "Game.h"

#include "Map.h"
#include "Camera.h"
#include "PhysicsEngine.h"
#include "TiledConverter.h"
#include "ObjectLibrary.h"
#include "SidebarRenderer.h"
#include "AchievementSystem.h"

#include "Text.h"
#include "InputHandler.h"
#include "entities\Player.h"

#include "SpriteManager.h"

APPLICATION_ENTRY(Game);

bool Game::onCreate(int a_argc, char* a_argv[])
{
	// Initialize information needed for the configuration
	setBackgroundColor(NHTV::SColour(0x00, 0x00, 0x00, 0xFF));
	GetScreenSize(m_screenWidth,m_screenHeight);
	
	std::srand((unsigned)std::time(0));
	std::rand();

	// Initialize subsystems
	// Set up camera
	m_camera = new Camera(m_CameraMatrix, m_screenWidth, m_screenHeight);
	m_camera->setZoom(2.f);

	// Set up tile converter
	std::vector<std::vector<unsigned>> map;
	m_tiledConverter = new TiledConverter();
	m_tiledConverter->importTiled(map);
	
	// Set up object map
	m_objectLibrary = new ObjectLibrary(); // Map needs an instance of this

	float newWidth, newHeight;
	m_camera->getScreenSize(newWidth, newHeight);
	m_map = new Map((int)newWidth,(int)newHeight,32,32,m_objectLibrary);
	m_map->setTiledConverterHandle(m_tiledConverter);
	
	// Set up physics engine
	m_physicsEngine = new PhysicsEngine(*m_map);

	// Set up object library and load initial level
	m_objectLibrary->initialize(m_map, m_physicsEngine);
	m_objectLibrary->setHeroClass(3);
	m_objectLibrary->setMonsterLevel(1);
	m_map->loadMap(map,true);

	// Set window handle for input handler
	InputHandler::getInstance().m_app = this;

	// Set up player id tracking
	m_playerID = m_map->getPlayerID();

	// Initialize text renderer
	Text::initialize();

	// Set up sidebar renderer;
	m_sidebarRenderer = new SidebarRenderer();
	glm::vec2 playerPos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_playerID,playerPos);
	m_sidebarRenderer->setPlayer(static_cast<Player*>(m_map->posToCell(playerPos)->object));

	// Set up achievement system
	m_achievementSystem = new AchievementSystem();
	m_physicsEngine->pushListener(m_achievementSystem);

	return true;
}

void Game::onUpdate(float a_deltaTime)
{
	if (a_deltaTime > 1)
		a_deltaTime = 1.f / 60.f;

	// quit our application when escape is pressed
	if (IsKeyDown(NHTV::KEY_ESCAPE))
		destroy();

	if (m_achievementSystem->updateElapsedTime(a_deltaTime)) return;
	
	// Set map to player position
	m_map->update(a_deltaTime);

	// Update the map
	float xPos = 0; float yPos = 0;
	GetSpritePosition(m_playerID, xPos, yPos);
	m_map->setCenterPos(xPos, yPos);
	m_map->updateView();
	
	// Handle collisions and update objects
	m_physicsEngine->moveObjects();
	m_map->updateAfterMove(a_deltaTime);

	// Update camera to map position
	// Map position makes sure that camera 
	// doesn't leave playing area, this is because
	// map position is made to not leave the map view
	GetSpritePosition(m_playerID, xPos, yPos);
	m_map->setCenterPos(xPos, yPos);
	m_map->getCenterPos(xPos,yPos);
	m_camera->setPosition(xPos,yPos);
	m_sidebarRenderer->setPosition(glm::vec2(xPos, yPos));
	m_sidebarRenderer->setCurrentLevel(m_tiledConverter->getCurrentLevel());
	m_achievementSystem->setTextDrawPosition(glm::vec2(xPos, yPos));

	// Check if player is dead
	glm::vec2 playerPos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_playerID, playerPos);
	Map::Data * player = static_cast<Map::Data*>(m_map->posToCell(playerPos));
	if (static_cast<Entity*>(player->object)->getHealth() <= 0.f)
	{
		m_tiledConverter->setCurrentLevel(0);
		delete player->object;
		player->object = nullptr;

		std::vector<std::vector<unsigned>> map;
		m_tiledConverter->importTiled(map);
		m_map->loadMap(map, true);
		m_playerID = m_map->getPlayerID();

		glm::vec2 playerPos;
		NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_playerID, playerPos);
		m_sidebarRenderer->setPlayer(static_cast<Player*>(m_map->posToCell(playerPos)->object));
		m_achievementSystem->resetAchievements();
	}

	// Temp reload level
	if (IsKeyDown(NHTV::KEY_P))
	{
		std::vector<std::vector<unsigned>> map;
		m_tiledConverter->importTiled(map);
		m_map->loadMap(map);
		m_playerID = m_map->getPlayerID();
	}
}

void Game::onDraw()
{
	clearScreen();

	m_map->draw();

	if(!m_achievementSystem->updateElapsedTime(0))
		m_sidebarRenderer->draw();
	m_achievementSystem->draw();
	Text::display();
}

void Game::onDestroy()
{
	delete m_map;
	delete m_camera;
	delete m_physicsEngine;
	delete m_tiledConverter;
	delete m_objectLibrary;
	delete m_sidebarRenderer;
	delete m_achievementSystem;
}