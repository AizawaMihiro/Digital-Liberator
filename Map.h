#pragma once
#include "Block.h"
#include "MinerMaze.h"

namespace {
	const int DEF_MAP_SIZE = 15;
	enum generator {
		STICK,DIG,WALL
	};
}

class Map
{
public:
	Map();
	Map(generator gen);
	~Map();
	void Instantinate();
	void Draw();	
private:
	int map[DEF_MAP_SIZE][DEF_MAP_SIZE] = { 0 };
	int hBlockModel;
	MinerMaze* maze_;
	std::vector<std::vector<int>> MapData;
};

