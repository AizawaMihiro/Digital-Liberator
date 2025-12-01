#pragma once
#include "Maze.h"
class MinerMaze :
    public Maze
{
public:
	MinerMaze();
	MinerMaze(int w, int h);
	 ~MinerMaze();
	 void Instantiate() override;
	 void Update() override;
	 void Draw() override;
	 std::vector<std::vector<int>> GetGrid() { return grid_; }
private:
	void DigSystem(int r, int c);
};

