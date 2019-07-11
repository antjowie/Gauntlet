#include "Map.h"
#include "ObjectLibrary.h"
#include "objects/Object.h"
#include "objects/AdjacentObject.h"

#include "SpriteManager.h"
#include "Utilities.h"

#include <iostream>

Map::Map(const int screenWidth, const int screenHeight, const int cellWidth, const int cellHeight, ObjectLibrary * objLib):
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_cellWidth(cellWidth),
	m_cellHeight(cellHeight),
	m_viewX(0),
	m_viewY(0),
	m_objLib(objLib),
	// We need an offset because we calculate the tiles on screen
	// We don't always have an even number of tiles on the screen, so we offset it by
	// a value just to make sure that we always display tiles.
	m_tileCountX(screenWidth / cellWidth + 1), 
	// The y coordinate has some weird behaviour. It's probably because of the resolution that we are using,
	// but it won't go further than half a tile. Just to make things easy, I'll add 1 extra.
	m_tileCountY(screenHeight/cellHeight + 2),
	m_newMapLoaded(false)
{
	updateView();
}

Map::~Map()
{
	clear();
}

void Map::loadMap(const std::vector<std::vector<unsigned>> &map, bool forceLoad)
{
	m_mapToLoad = map;
	m_newMapLoaded = true;
	if (forceLoad)
		loadNewMap();
}

void Map::pushProjectile(Object * obj)
{
	m_projectilesToAdd.push_back(obj);
}

void Map::updateView()
{
	m_iterX[0] = NHTV::Clamp(int(m_viewX - m_screenWidth * 0.5f) / (int)m_cellWidth, 0, (int)m_map.size());
	m_iterX[1] = NHTV::Clamp((int)m_iterX[0] + m_tileCountX, 0, (int)m_map.size());
	// Y coordinated are flipped. For the vector 0 is above
	m_iterY[0] = NHTV::Clamp((int)m_map.size() - int(m_viewY + (float)m_screenHeight * 0.5f) / (int)m_cellHeight - 1, 0, (int)m_map.size());
	m_iterY[1] = NHTV::Clamp((int)m_iterY[0]+m_tileCountY, 0, (int)m_map.size());
}

void Map::setCenterPos(const float x, const float y)
{
	m_viewX = NHTV::Clampf(x, m_screenWidth*0.5f, m_map.size()*m_cellWidth - m_screenWidth * 0.5f);
	m_viewY = NHTV::Clampf(y, m_screenHeight*0.5f, m_map.size()*m_cellHeight - m_screenHeight* 0.5f);
}

void Map::getCenterPos(float & x, float & y) const
{
	x = m_viewX;
	y = m_viewY;
}

Map::Data * Map::posToCell(const glm::vec2 & pos)
{
	float x = pos.x;
	float y = pos.y;

	mapPosToIndex(x, y);
	return &m_map[(size_t)y][(size_t)x];
}

std::vector<std::vector<Map::Data*>> Map::getSurroundingTiles(float xPos, float yPos)
{
	std::vector<std::vector<Data*>> data;

	float i = xPos, j = yPos;
	mapPosToIndex(i, j);

	for (int count = 0, y = (int)j - 1; y <= (int)j + 1; y++)
	{
		data.push_back(std::vector<Data*>());
		for (int x = (int)i - 1; x <= (int)i + 1; x++)
		{
			// If out of bounds
			if (x < 0 || x >= m_map.size() || y < 0 || y >= m_map.size())
				data[count].push_back(nullptr);
			else 
				data[count].push_back(&m_map[y][x]);
		}
		count++;
	}

	return data;
}

unsigned Map::getPlayerID() const
{
	for (std::vector<std::vector<Data>>::const_iterator y = m_map.begin(); y != m_map.end(); y++)
		for (std::vector<Data>::const_iterator x = y->begin(); x != y->end(); x++)
			if (x->object && x->object->m_type == Object::Hero)
				return x->object->m_id;
	return 0;
}

void Map::setTiledConverterHandle(TiledConverter * tiledConverter)
{
	m_tiledConverterHandle = tiledConverter;
}

TiledConverter * Map::getTiledConverterHandle()
{
	return m_tiledConverterHandle;
}

void Map::loadNewMap()
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();

	// Check if player already exists
	unsigned playerID = getPlayerID();
	Object * player = nullptr;
	if (playerID != 0)
	{
		glm::vec2 playerPos;
		sprite->GetSpritePosition(playerID, playerPos);
		Data * data = posToCell(playerPos);
		player = data->object;
		data->object = nullptr;
	}

	clear();

	m_map.reserve(m_mapToLoad.size());
	for (size_t y = 0; y < m_mapToLoad.size(); y++)
	{
		m_map.push_back(std::vector<Data>());
		m_map[y].reserve(m_mapToLoad[y].size());
		for (size_t x = 0; x < m_mapToLoad[y].size(); x++)
		{
			Data data;
			glm::vec2 pos;

			// Load the object
			Object::Type objType = (Object::Type)m_mapToLoad[y][x];
			Object * object;

			// If player is already in game, load player instead of new player
			if (objType == 15 && player) // Tiled maps 1 to the object type 'Hero'
				object = player;
			else
				object = m_objLib->getObject(objType);

			pos.x = (float)x * m_cellWidth + m_cellWidth * 0.5f;
			pos.y = (float)m_mapToLoad.size() * m_cellHeight - y * m_cellHeight - m_cellHeight * 0.5f;
			sprite->MoveSprite(object->m_id, glm::value_ptr(pos));

			if (object->m_type == Object::Type::Floor)
			{
				data.object = nullptr;
				data.tile = object;
			}
			// Because no tile is loaded, just load a tile
			else
			{
				data.object = object;

				// Create a tile for the background
				object = m_objLib->getObject((Object::Type)36); // This is a floor tile
				// Load the tile
				pos.x = (float)x * m_cellWidth + m_cellWidth * 0.5f;
				pos.y = (float)m_mapToLoad.size() * m_cellHeight - y * m_cellHeight - m_cellHeight * 0.5f;
				sprite->MoveSprite(object->m_id, glm::value_ptr(pos));
				data.tile = static_cast<AdjacentObject*>(object);
				data.tile = object;
			}

			data.isClaimed = false;
			m_map[y].push_back(data);
		}
	}

	// Fix adjecent textures and shadows
	for (std::vector<std::vector<Data>>::iterator row = m_map.begin(); row != m_map.end(); row++)
		for (std::vector<Data>::iterator elem = row->begin(); elem != row->end(); elem++)
		{
			if (elem->object && (elem->object->m_type == Object::Wall || elem->object->m_type == Object::Gate))
				static_cast<AdjacentObject*>(elem->object)->setCorrectFrame();
    			static_cast<AdjacentObject*>(elem->tile)->setCorrectFrame();
		}

	m_newMapLoaded = false;
}

void Map::mapPosToIndex(float & x, float & y) const
{
	// Map pos it to cell map
	x = (x) / m_cellWidth;
	y = (m_map.size()*m_cellHeight - y) / m_cellHeight;
	
	// Make sure the value is in bounds 
	x = NHTV::Clampf(x, 0.f, (float)m_map.size());
	y = NHTV::Clampf(y, 0.f, (float)m_map.size());
}

void Map::update(const float elapsedTime)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();

	// Update object map
	for (size_t y = m_iterY[0], j = 0; y < m_iterY[1]; y++, j++)
		for (size_t x = m_iterX[0], i = 0; x < m_iterX[1]; x++, i++)
		{
			Object * obj = m_map[y][x].object;
			if (obj != nullptr)
			{
				// The reason why we update the object after we check if its deleted is because
				// it requests a movement in the physics engine. If it requests a movement and is dead in 
				// the same frame. The program would crash. Now the object is deleted only on the next frame which isn't too
				// bad becasue the physics engine only checks alive objects
				if (obj->getDead())
				{
					delete obj;
					m_map[y][x].object = nullptr;
				}
				else
					obj->update(elapsedTime);
			}
		}

	// Update vectors
	m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(), [](Object *obj)
	{
		bool dead = obj->getDead();
		if (dead)
			delete obj;
		return dead;
	}), m_projectiles.end());

	// Add the things that have to be added
	m_projectiles.insert(m_projectiles.end(), m_projectilesToAdd.begin(), m_projectilesToAdd.end());
	m_projectilesToAdd.clear();

	// Update the vectors
	for (std::vector<Object*>::iterator elem = m_projectiles.begin(); elem != m_projectiles.end(); elem++)
		(*elem)->update(elapsedTime);
}

void Map::updateAfterMove(const float elapsedTime)
{
	for (size_t y = m_iterY[0], j = 0; y < m_iterY[1]; y++, j++)
		for (size_t x = m_iterX[0], i = 0; x < m_iterX[1]; x++, i++)
		{
			Object * obj = m_map[y][x].object;
			if (obj != nullptr)
			{
				if (obj->getDead())
				{
					delete obj;
					m_map[y][x].object = nullptr;
				}
				else
					obj->updateAfterMove(elapsedTime);
			}
		}

	// Check if new map has to be loaded
	if (m_newMapLoaded)
		loadNewMap();
}

void Map::draw() const
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();

	// Objects have to be drawn after all the tiles are drawn
	for (int i = 0; i < 2; i++)
	{
		for (size_t y = m_iterY[0]; y < m_iterY[1]; y++)
			for (size_t x = m_iterX[0]; x < m_iterX[1]; x++)
			{
				// We first draw all bottom tiles (floor tiles). 
				// After that we draw the objects like walls. This is because objects
				// moving down have to be drawn over tiles and not under them
				switch (i)
				{
				case 0:
					sprite->MarkSpriteForDraw(m_map[y][x].tile->m_id);
					break;
					
				case 1:
					if (m_map[y][x].object != nullptr)
						sprite->MarkSpriteForDraw(m_map[y][x].object->m_id);
					break;
				}
			}
	}

	for (std::vector<Object*>::const_iterator elem = m_projectiles.begin(); elem != m_projectiles.end(); elem++)
		sprite->MarkSpriteForDraw((*elem)->m_id);

	//std::printf("VIEW: X[%i-%i] Y[%i-%i]\n", (int)m_iterX[0], (int)m_iterX[1], 32 - (int)m_iterY[1], 32 - (int)m_iterY[0]);
}

void Map::clear()
{
	// Clear map
	for (size_t y = 0; y < m_map.size(); y++)
	{
		for (size_t x = 0; x < m_map[y].size(); x++)
		{
			if (m_map[y][x].tile != nullptr)
				delete m_map[y][x].tile;
			if (m_map[y][x].object != nullptr)
				delete m_map[y][x].object;
		}
		m_map[y].clear();
	}
	m_map.clear();

	// Clear projectiles
	m_projectiles.insert(m_projectiles.end(), m_projectilesToAdd.begin(), m_projectilesToAdd.end());
	m_projectilesToAdd.clear();
	for (std::vector<Object*>::iterator elem = m_projectiles.begin(); elem != m_projectiles.end(); elem++)
		delete *elem;
	m_projectiles.clear();
}