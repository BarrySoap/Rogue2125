#include "cmp_tilemap.h"
#include "system_physics.h"
//#include "maths.h"

using namespace std;
using namespace sf;

TileMapComponent::TileMapComponent(Entity* p, TextureMgr* txrMgr, int currMap)
	: Component(p)
{
	m_txrMgr = txrMgr;

	m_width = 40 + ((currMap) * 3);
	m_height = 40 + ((currMap) * 3);

	m_ID = "map" + currMap;
	m_IntID = currMap;

	m_roomIDs = RandomInt(1, 5);
	m_corridorIDs = RandomInt(1, 5);

	while (m_corridorIDs == m_roomIDs)
	{
		m_corridorIDs = RandomInt(1, 5);
	}
	

	m_tileEnts = vector<vector<shared_ptr<Entity>>>();
	m_tileCmps = vector<vector<shared_ptr<TileComponent>>>();
	m_visibleTiles = vector<vector<shared_ptr<TileComponent>>>();

	m_rooms = vector<IntRect>();

	m_corridorDirs = vector<int>();
	m_corridors = vector<vector<shared_ptr<TileComponent>>>();

	bool b = true;
	if (!b)
	{

	}
	else
	{
		for (int y = 0; y < m_height; y++)
		{
			m_tileEnts.push_back(vector<shared_ptr<Entity>>());
			m_tileCmps.push_back(vector<shared_ptr<TileComponent>>());
		}

		for (int y = 0; y < m_tileEnts.size(); y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				shared_ptr<Entity> ent = make_shared<Entity>(nullptr);
				ent->setPosition(Vector2f(16.0f + (32.0f * x), 16.0f + (32.0f * y)));
				
				shared_ptr<TileComponent> cmpT = ent->addComponent<TileComponent>(txrMgr, x, y);
				cmpT->SetTileID(9);
				//cmpT->SetColor(m_txrMgr->colors_LiquidTiles[0]);
				cmpT->update(1.0f);

				m_tileEnts[y].push_back(ent);
				m_tileCmps[y].push_back(cmpT);
			}
		}

		GenerateMap();

		m_visibleTiles = GetTiles(IntRect(0, 0, m_width, m_height));
	}
}

void TileMapComponent::update(double dt)
{

}

void TileMapComponent::render()
{
	for (int y = 0; y < m_visibleTiles.size(); y++)
	{
		for (int x = 0; x < m_visibleTiles[y].size(); x++)
		{
			m_visibleTiles[y][x]->render();
		}
	}
}

string TileMapComponent::GetID() { return m_ID; }
int TileMapComponent::GetIntID() { return m_IntID; }

shared_ptr<TileComponent> TileMapComponent::GetTile(int x, int y)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_tileCmps[y][x];
	}
	else
	{
		return nullptr;
	}
}

shared_ptr<Entity> TileMapComponent::GetEnt(int x, int y)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_tileEnts[y][x];
	}
	else
	{
		return nullptr;
	}
}

vector<vector<shared_ptr<TileComponent>>> TileMapComponent::GetTiles(IntRect area)
{
	vector<vector<shared_ptr<TileComponent>>> tilegrid = vector<vector<shared_ptr<TileComponent>>>();

	for (int i = 0; i < area.height; i++)
	{
		tilegrid.push_back(vector<shared_ptr<TileComponent>>());
	}

	bool areaValid = (area.left >= 0 && area.left + area.width - 1 < m_width) && (area.top >= 0 && area.top + area.height - 1 < m_height);

	if (areaValid)
	{
		int sX = area.left;
		int sY = area.top;

		for (int y = 0; y < area.height; y++)
		{
			for (int x = 0; x < area.width; x++)
			{
				int cX = sX + x;
				int cY = sY + y;

				tilegrid[y].push_back(GetTile(cX, cY));
			}
		}

		return tilegrid;
	}
	else
	{
		return tilegrid;
	}
}
vector<vector<shared_ptr<TileComponent>>> TileMapComponent::GetNeighbourTiles(int x, int y)
{
	vector<vector<shared_ptr<TileComponent>>> tilegrid = vector<vector<shared_ptr<TileComponent>>>();

	for (int i = 0; i < 2; i++)
	{
		tilegrid.push_back(vector<shared_ptr<TileComponent>>());
	}

	if (y - 1 >= 0)
		tilegrid[0].push_back(GetTile(x, y - 1));
	else
		tilegrid[0].push_back(nullptr);

	if (y + 1 < m_height)
		tilegrid[0].push_back(GetTile(x, y + 1));
	else
		tilegrid[0].push_back(nullptr);

	if (x - 1 >= 0)
		tilegrid[1].push_back(GetTile(x - 1, y));
	else
		tilegrid[1].push_back(nullptr);

	if (x + 1 < m_width)
		tilegrid[1].push_back(GetTile(x + 1, y));
	else
		tilegrid[1].push_back(nullptr);

	return tilegrid;
}

vector<vector<shared_ptr<Entity>>> TileMapComponent::GetNeightbourEnts(int X, int Y)
{
	vector<vector<shared_ptr<Entity>>> tilegrid = vector<vector<shared_ptr<Entity>>>();

	for (int i = 0; i < 3; i++)
	{
		tilegrid.push_back(vector<shared_ptr<Entity>>());
	}

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			int cX = X + x;
			int cY = Y + y;
			int vY = y + 1;

			tilegrid[vY].push_back(GetEnt(cX, cY));
		}
	}

	return tilegrid;
}

vector<vector<shared_ptr<TileComponent>>> TileMapComponent::GetAllNeighbourTiles(int X, int Y)
{
	vector<vector<shared_ptr<TileComponent>>> tilegrid = vector<vector<shared_ptr<TileComponent>>>();

	for (int i = 0; i < 3; i++)
	{
		tilegrid.push_back(vector<shared_ptr<TileComponent>>());
	}

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			int cX = X + x;
			int cY = Y + y;
			int vY = y + 1;

			tilegrid[vY].push_back(GetTile(cX, cY));
		}
	}

	return tilegrid;
}

bool TileMapComponent::LoadMap()
{
	return false;
}
//void TileMapComponent::SaveMap()
//{
//
//}
//
void TileMapComponent::GenerateMap()
{
	int rRooms = sf::RandomInt(7 + (m_IntID / 2), 11 + (m_IntID / 2));
	IntRect prevRoom;
	IntRect currRoom;

	// Generate first room
	int rW = sf::RandomInt(3, 8);
	int rH = sf::RandomInt(3, 8);
	prevRoom = IntRect(sf::RandomInt(1, m_width - rW - 2), sf::RandomInt(1, m_height - rH - 2), rW, rH);

	m_rooms.push_back(prevRoom);

	for (int i = 0; i < rRooms; i++)
	{
		currRoom = GenerateRoom(prevRoom);
		m_rooms.push_back(currRoom);
		prevRoom = currRoom;
	}

	for (int i = 0; i < m_corridors.size(); i++)
	{
		AlterCorridorTiles(m_corridors[i]);
	}
	for (int i = 0; i < m_rooms.size(); i++)
	{
		AlterRoomTiles(m_rooms[i]);
	}
	IterateAcrossTileMap();
}

char TileMapComponent::CalculateTileIdx(vector<vector<shared_ptr<TileComponent>>> neighbours)
{
	char idx = 0;

	if (neighbours[0][0] != nullptr)
	{
		if (!neighbours[0][0]->GetWalkable() || neighbours[0][0]->GetDisguised())
		{
			idx += 1;
		}
	}
	else
	{
		idx += 1;
	}

	if (neighbours[0][1] != nullptr)
	{
		if (!neighbours[0][1]->GetWalkable() || neighbours[0][1]->GetDisguised())
		{
			idx += 4;
		}
	}
	else
	{
		idx += 4;
	}

	if (neighbours[1][0] != nullptr)
	{
		if (!neighbours[1][0]->GetWalkable() || neighbours[1][0]->GetDisguised())
		{
			idx += 8;
		}
	}
	else
	{
		idx += 8;
	}

	if (neighbours[1][1] != nullptr)
	{
		if (!neighbours[1][1]->GetWalkable() || neighbours[1][1]->GetDisguised())
		{
			idx += 2;
		}
	}
	else
	{
		idx += 2;
	}

	return idx;
}

bool TileMapComponent::ToggleTile(vector<vector<shared_ptr<TileComponent>>> tiles)
{
	for (int y = 0; y < tiles.size(); y++)
	{
		for (int x = 0; x < tiles[y].size(); x++)
		{
			if (tiles[y][x] != nullptr && tiles[y][x]->GetWalkable())
				return true;
		}
	}

	return false;
}

IntRect TileMapComponent::GenerateRoom(IntRect prev)
{


	int rW = sf::RandomInt(3, 8);
	int rH = sf::RandomInt(3, 8);

	IntRect newRoom;
	newRoom = IntRect(sf::RandomInt(1, m_width - rW - 2), sf::RandomInt(1, m_height - rH - 2), rW, rH);

	if (newRoom.left <= 0)
		newRoom.left = 1;
	else if (newRoom.left + newRoom.width >= m_width)
		newRoom.left = m_width - newRoom.width - 1;

	if (newRoom.top <= 0)
		newRoom.top = 1;
	else if (newRoom.top + newRoom.height >= m_height)
		newRoom.top = m_height - newRoom.height - 1;

	m_corridors.push_back(GenerateCorridor(prev, newRoom));

	return newRoom;
}

std::vector<std::shared_ptr<TileComponent>> TileMapComponent::GenerateCorridor(IntRect r1, IntRect r2)
{
	std::vector<std::shared_ptr<TileComponent>> corridor = std::vector<std::shared_ptr<TileComponent>>();

	// Get room centres.
	Vector2i r1C = Vector2i(r1.left + (r1.width / 2), r1.top + (r1.height / 2));
	Vector2i r2C = Vector2i(r2.left + (r2.width / 2), r2.top + (r2.height / 2));

	int curr[2];
	int gDirs[2] = { 0, 0 };

	curr[0] = r1C.x;
	curr[1] = r1C.y;

	bool flag = false;

	// Generate corridor via multiple "halls"
	while (curr[0] != r2C.x || curr[1] != r2C.y)
	{
		int dS[2] = { 0, 0 };
		dS[0] = r2C.x - curr[0];
		dS[1] = r2C.y - curr[1];

		if (dS[0] != 0)
			gDirs[0] = dS[0] / abs(dS[0]);
		else
			gDirs[0] = 0;

		if (dS[1] != 0)
			gDirs[1] = dS[1] / abs(dS[1]);
		else
			gDirs[1] = 0;

		int dir = RandomInt(0, 2);

		// Ensures that if the chosen direction cannot over shoot the target room centre.
		while (abs(dS[dir]) <= 0)
		{
			dir = RandomInt(0, 2);
		}

		// Ensures that length of next "hall" cannot over shoot target.
		int length = RandomInt(min(1, abs(dS[dir])), min(12, abs(dS[dir])));

		int dest = curr[dir] + (length * gDirs[dir]);

		// Create "hall"
		while (curr[dir] != dest)
		{
			corridor.push_back(GetTile(curr[0], curr[1]));
			curr[dir] += gDirs[dir];
		}
	}

	return corridor;
}

void TileMapComponent::AlterRoomTiles(IntRect room)
{
	vector<vector<shared_ptr<TileComponent>>> tilegrid = GetTiles(room);

	for (int y = 0; y < tilegrid.size(); y++)
	{
		for (int x = 0; x < tilegrid[y].size(); x++)
		{
			tilegrid[y][x]->SetColor(Color(255, 255, 255, 255));
			tilegrid[y][x]->SetTileID(3);
		}
	}
}

void TileMapComponent::AlterCorridorTiles(std::vector<std::shared_ptr<TileComponent>> corridor)
{
	for (int i = 0; i < corridor.size(); i++)
	{
		corridor[i]->SetColor(Color(255, 255, 255, 255));
		corridor[i]->SetTileID(1);
	}
}

void TileMapComponent::IterateAcrossTileMap()
{
	for (int y = 0; y < m_tileEnts.size(); y++)
	{
		for (int x = 0; x < m_tileEnts[y].size(); x++)
		{
			if (!m_tileCmps[y][x]->GetWalkable())
				m_tileCmps[y][x]->SetTileIdx(1 + CalculateTileIdx(GetNeighbourTiles(x, y)));

			bool vis = ToggleTile(GetAllNeighbourTiles(x, y));
			m_tileEnts[y][x]->setVisible(vis);
		}
	}
}