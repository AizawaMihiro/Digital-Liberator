#pragma once
#include <vector>
#include <list>

enum maze
{
	ROAD = 0, 
	WALL = 1, 
	START = 2, 
	GOAL = 3, 
	PILLAR = 4,
	PATH = 5
};

namespace {
	const int TILE_SIZE = 32;
}

class Maze
{
public:
	Maze(int w, int h);
	virtual ~Maze();
	virtual void Instantiate() =0;
	virtual void Update() =0;
	virtual void Draw() =0;
	int dir[4][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };// è„â∫ç∂âE
protected:
	int width_;
	int height_;
	std::vector<std::vector<int>> grid_;	
};

