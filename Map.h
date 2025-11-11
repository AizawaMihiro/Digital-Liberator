#pragma once
#include "Block.h"

namespace {
	const int DEF_MAP_SIZE = 20;
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
private:
	int map[DEF_MAP_SIZE][DEF_MAP_SIZE] = { 0 };
};

