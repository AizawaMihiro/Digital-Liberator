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
	/// <summary>
	/// 再帰的に穴掘り法で迷路を生成する関数
	/// </summary>
	/// <param name="r">掘りを開始するy座標</param>
	/// <param name="c">掘りを開始するx座標</param>
	void DigSystem(int r, int c);
};

