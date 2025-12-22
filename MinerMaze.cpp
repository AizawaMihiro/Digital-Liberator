#include "MinerMaze.h"
#include <DxLib.h>

MinerMaze::MinerMaze()
	: MinerMaze(10, 10)
{
}

MinerMaze::MinerMaze(int w, int h)
	: Maze(w, h)
{
}

MinerMaze::~MinerMaze()
{
}

void MinerMaze::Instantiate()
{
	//生成処理
	// 初期化
	for(auto &row:grid_)
	{
		for (auto& cell : row)
		{
			cell = maze::WALL;
		}
	}
	// 開始地点を設定
	// ランダムに奇数座標を選ぶ
	int r = (rand() % height_/2) * 2;
	int c = (rand() % width_/2) * 2;
	if (r%2==0)
	{
		r += 1;
		if (r==height_)
		{
			r -= 2;
		}
	}
	if (c % 2 == 0)
	{
		c += 1;
		if (c == width_)
		{
			c -= 2;
		}
	}
	grid_[r][c] = maze::LOAD;

	// 穴掘り法処理を実装した関数で再起処理を行う
	DigSystem(r, c);
}

void MinerMaze::Update()
{
	//更新処理
	// 特に無し
}

void MinerMaze::Draw()
{
	//描画処理
	for (int r = 0; r < height_; r++)
	{
		for (int c = 0; c < width_; c++)
		{
			if (grid_[r][c] == maze::LOAD)
			{
				DrawBox(c * TILE_SIZE, r * TILE_SIZE, (c + 1) * TILE_SIZE, (r + 1) * TILE_SIZE, GetColor(255, 255, 255), TRUE);
			}
		}
	}
}

void MinerMaze::DigSystem(int r, int c)
{
	//穴掘り法の実装
	// ランダムに方向を決定
	int randDir[4][2];
	for (int i = 0;i < 4;i++)
	{
		int randIndex = rand() % 4;
		randDir[i][0] = dir[randIndex][0];
		randDir[i][1] = dir[randIndex][1];
	}

	// 4方向に対して掘削を試みる
	for (int i = 0;i < 4;i++)
	{
		int dr = randDir[i][0];//direction row
		int dc = randDir[i][1];//direction column
		int nr = r + dr;// new row
		int nc = c + dc;// new column
		int sr = r + dr * 2;// second row
		int sc = c + dc * 2;// second column
		
		// 掘削可能かチェック
		if (sr<1 || sr >=height_)//上下
		{
			continue;
		}
		if (sc<1 || sc >=width_)//左右
		{
			continue;
		}
		if (grid_[nr][nc] == maze::LOAD)// すでに掘られている
		{
			continue;
		}
		if (grid_[sr][sc] == maze::LOAD)// 2マス先が掘られている
		{
			continue;
		}
		// 掘削実行
		grid_[nr][nc] = maze::LOAD;
		grid_[sr][sc] = maze::LOAD;// 2マス先も掘る 見栄えが良くなる
		// 再起処理
		DigSystem(sr, c + dc * 2);
	}

}
