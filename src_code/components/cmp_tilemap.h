#pragma once

#include "cmp_tile.h"
#include "AllMgrs.h"
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class TileMapComponent : public Component
{
public:
	TileMapComponent() = delete;

	explicit TileMapComponent(Entity* p, TextureMgr* txrMgr, int currMap);
	void GenerateOtherMap(int map);

	void update(double dt) override;
	void render() override;

	bool LoadMap();
	void SaveMap();

	char ConvertFromInt(int n);
	int ConvertFromChar(char c);

	void GenerateMap();

	std::string GetID();
	int GetIntID();
	std::shared_ptr<TileComponent> GetTile(int x, int y);
	std::shared_ptr<TileComponent> GetTile(sf::Vector2i xy);
	std::shared_ptr<Entity> GetEnt(int x, int y);
	std::vector<std::vector<std::shared_ptr<TileComponent>>> GetTiles(sf::IntRect area);
	std::vector<std::vector<std::shared_ptr<TileComponent>>> GetNeighbourTiles(int x, int y);
	std::vector<std::vector<std::shared_ptr<TileComponent>>> GetAllNeighbourTiles(int X, int Y);

	std::vector<std::vector<std::shared_ptr<TileComponent>>> GetAllNeighbourWalls(int X, int Y);	// Used in similar fashion as Get

	std::vector<std::vector<std::shared_ptr<Entity>>> GetAllNeightbourEnts(int X, int Y);			// Used to get neighbouring entities to toggle visibility.

	bool GetNeighbourWalls(int x, int y, int &walls);	// Used to check if in corridor

	void GenerateMapObjs();
	void GenerateFromScratch();

	void SetVisibleTiles();
protected:

private:
	TextureMgr* m_txrMgr;

	std::string m_ID;
	int m_IntID;
	int m_width;
	int m_height;
	bool m_flag;

	char m_roomIDs;
	char m_corridorIDs;

	float m_chanceOfLiquid;

	sf::Color m_roomColor;
	sf::Color m_corridorColor;

	std::vector<std::vector<std::shared_ptr<Entity>>> m_tileEnts;
	std::vector<std::vector<std::shared_ptr<TileComponent>>> m_tileCmps;
	std::vector<std::vector<std::shared_ptr<TileComponent>>> m_visibleTiles;



	std::vector<sf::IntRect> m_rooms;

	std::vector<int> m_corridorDirs;
	std::vector<std::vector<std::shared_ptr<TileComponent>>> m_corridors;

	char CalculateTileIdx(std::vector<std::vector<std::shared_ptr<TileComponent>>> neighbours, std::shared_ptr<TileComponent> tile);
	char CalculateLiquidID(std::vector<std::vector<std::shared_ptr<TileComponent>>> neighbours, char currID);

	bool CheckForRoughLiquid(std::vector<std::vector<std::shared_ptr<TileComponent>>> neighbours, char currID);
	void SmoothLiquids(int bX, int bY);

	bool ToggleTile(std::vector<std::vector<std::shared_ptr<TileComponent>>> tiles);

	sf::IntRect GenerateRoom(sf::IntRect prev);	// Generate new room within bounds of previous.
	std::vector<std::shared_ptr<TileComponent>> GenerateCorridor(sf::IntRect r1, sf::IntRect r2);	// Generate corridor from one room to another.
	//sf::IntRect GenerateLockedRoom();			// Generate new room along corridor that is locked.
	//sf::IntRect GenerateHiddenRoom();			// Generate hidden room on side of normal room.

	void AlterRoomTiles(sf::IntRect room, char ID, sf::Color color, bool isLiquid);
	void AlterCorridorTiles(std::vector<std::shared_ptr<TileComponent>> corridor, char ID, sf::Color color);
	void IterateAcrossTileMap(int pass);		// Determines visibility, determines walls and propagates fluids.

	void IterateTiles();						// Similar to IterateAcross, however does not propagate fluids.

	bool ListContains(std::vector<std::shared_ptr<TileComponent>> list, std::shared_ptr<TileComponent> t);
};