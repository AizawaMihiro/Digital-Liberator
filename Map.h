#pragma once
#include "Block.h"
#include "Enemy.h"
#include "MinerMaze.h"
#include <vector>

namespace {
	const int DEF_MAP_SIZE = 15;
	enum generator {
		STICK,DIG,WALL
	};
}

class TextMap;

class Map
{
public:
	Map();
	Map(generator gen);
	~Map();
	void Instantinate();
	void Update();
	void Draw();	
private:
	bool CheckHitBlock(VECTOR3 playerPos, VECTOR3 blockPos, VECTOR3 playerScale);
	int map[DEF_MAP_SIZE][DEF_MAP_SIZE] = { 0 };
	int hBlockModel;
	MinerMaze* maze_;
	TextMap* textMap_ = nullptr;
	std::vector<std::vector<int>> MapData;
	std::vector<Block*> blocks_;
	std::vector<Enemy*> enemies_;
	/// <summary>
	/// w’è’n“_‚©‚çÅ‚à‰“‚¢êŠ‚ğ’T‚·ŠÖ”
	/// </summary>
	/// <returns>std::pair first=Y second=X</returns>
	std::pair<int, int> FindMoredistantPoint(int startX, int startY);
};

