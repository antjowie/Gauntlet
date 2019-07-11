#pragma once
#include "Application.h"

// Forward declerations to all our subsystems
class Map;
class TiledConverter;
class Camera;
class PhysicsEngine;
class ObjectLibrary;
class SidebarRenderer;
class AchievementSystem;

class Game: public NHTV::Application
{
protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

private:

	// All our subsystems
	Map *m_map;
	Camera *m_camera;
	PhysicsEngine *m_physicsEngine;
	TiledConverter *m_tiledConverter;
	ObjectLibrary * m_objectLibrary;
	SidebarRenderer * m_sidebarRenderer;
	AchievementSystem * m_achievementSystem;

	unsigned m_playerID;

	int m_screenWidth;
	int m_screenHeight;
};