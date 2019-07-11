#pragma once
#include <vector>
#include <glm/vec2.hpp>

class Game;
class Object;
class PhysicsEngine;
class ObjectLibrary;
class TiledConverter;

// appropiate, I will see how this game performes with all these nullptr checks.
// The collision manager uses the map to only check surrounding objects.
class Map
{
public:
	// Physics engine needs access to this
	// Or we make map a friend of PhysicsEngine because
	// Physics Engine may know about map's internals
	struct Data;

	Map(const int screenWidth, const int screenHeight, const int cellWidth, const int cellHeight, ObjectLibrary * objLib);
	~Map();

	// The map consist of tile id's. It is most commonly supplied by the level loaded.
	// Force load will load the map in the same call
	void loadMap(const std::vector<std::vector<unsigned>> &map, bool forceLoad = false);
	
	// The projectile map is a map that contains all the projectiles
	// This is supplied to the map because the map is used to check for collision
	void pushProjectile(Object * obj);
	
	// This function updates the indexes that decide what part of the map is drawn.
	void updateView();

	// Decides what part of the map will be updated and drawn. This can be seen as the center camera
	void setCenterPos(const float x, const float y);
	void getCenterPos(float &x, float &y) const;

	Data* posToCell(const glm::vec2 &pos);

	void update(const float elapsedTime);
	void updateAfterMove(const float elapsedTime);
	void draw() const;

	void clear();

	struct Data
	{
		Object* tile;
		Object* object;

		bool isClaimed;
	};
	
	// This function is needed by spawners and physics manager
	// It goes from upper left to lower right
	std::vector<std::vector<Data*>> getSurroundingTiles(float xPos, float yPos);
	
	unsigned getPlayerID() const;

	void setTiledConverterHandle(TiledConverter *tiledConverter);
	TiledConverter *getTiledConverterHandle();

private:
	
	void loadNewMap();

	void mapPosToIndex(float &x, float &y) const;

	// Index 0:
	// This map is responsible for all the objects that are static.
	// Things like background objects. They can't be destroyed nor moved.
	//-----------------
	// Index 1:
	// This map is responsible for objects that can move
	std::vector<std::vector<Data>> m_map;

	// There is some code duplication here.
	// I could wrap it inside a class.
	std::vector<Object*> m_projectiles;
	std::vector<Object*> m_projectilesToAdd;

	// These values are used to calculate the area that the map has to draw.
	float			m_viewX;
	float			m_viewY;
	const unsigned	m_cellWidth;
	const unsigned	m_cellHeight;
	const int		m_screenWidth;
	const int		m_screenHeight;

	// These are the indexes that the map iterates over
	size_t		m_iterX[2];
	size_t		m_iterY[2];
	const int	m_tileCountX;
	const int	m_tileCountY;

	// Needed to create the objects
	ObjectLibrary * m_objLib;

	// Needed for level loading
	TiledConverter *m_tiledConverterHandle;

	bool m_newMapLoaded;
	std::vector<std::vector<unsigned>> m_mapToLoad;
};