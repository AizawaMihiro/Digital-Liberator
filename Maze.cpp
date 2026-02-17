#include "Maze.h"
#include <random>

Maze::Maze(int w, int h)
{
	width_ = w;
	height_ = h;
	grid_.resize(height_);
	for (int i = 0; i < height_; i++)
	{
		grid_[i].resize(width_);
	}

	srand((unsigned int)LimitTime(NULL));
}

Maze::~Maze()
{
	grid_.clear();
}
