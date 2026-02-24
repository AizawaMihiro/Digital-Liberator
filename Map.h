#pragma once
#include "Library/GameObject.h"
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

class Map :
	public GameObject
{
public:
	Map();
	Map(generator gen);
	~Map();
	void Instantinate();
	void Update();
	void Draw();
	bool GetGameClearFlag();
	bool GetGameOverFlag();
	std::vector<std::vector<int>> GetMapData() { return MapData; }
private:
	bool CheckHitBlock(VECTOR3 playerPos, VECTOR3 blockPos, VECTOR3 playerScale);
	bool CheckHitBlock(VECTOR3 cameraPos, VECTOR3 blockPos);
	std::pair<int, int> FindMoreDistantPoint(int startX, int startY);
	bool CheckErrorMaze();
	int map[DEF_MAP_SIZE][DEF_MAP_SIZE] = { 0 };
	int hBlockModel_;
	int hPillarModel_;
	MinerMaze* maze_;
	TextMap* textMap_ = nullptr;
	std::vector<std::vector<int>> MapData;
	std::vector<Block*> blocks_;
	std::vector<Enemy*> enemies_;
	bool gameOverFlag;
};

