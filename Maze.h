#pragma once
#include <vector>
#include <list>

namespace maze
{
	const int LOAD = 0;
	const int WALL = 1;
}

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
protected:
	int dir[4][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };// è„â∫ç∂âE
	int width_;
	int height_;
	std::vector<std::vector<int>> grid_;	
};

