#include "Map.h"

/// <summary>
/// ステージである迷路を生成するクラス
/// 与える因数によって生成方法を変更する
/// </summary>

Map::Map()
	:Map(DIG)
{
	hBlockModel = MV1LoadModel("Assets/model/Blockcolord.mv1");
}

Map::Map(generator gen)
{
	switch (gen)
	{
	case STICK:
		break;
	case DIG:
		maze_ = new MinerMaze(DEF_MAP_SIZE, DEF_MAP_SIZE);
		maze_->Instantiate();
		break;
	case WALL:
		break;
	default:
		break;
	}
}

Map::~Map()
{
	delete maze_;
}

void Map::Instantinate()
{
	MapData = maze_->GetGrid();
}

void Map::Draw()
{
	for (int r = 0; r < DEF_MAP_SIZE; r++)
	{
		for (int c = 0; c < DEF_MAP_SIZE; c++)
		{
			if (MapData[r][c] == maze::WALL)
			{
				Block* block = new Block(hBlockModel);
				block->SetPosition(VECTOR3{ c * BLOCK::SIZE*2 ,0.0f,r * BLOCK::SIZE*2 });
				block->Draw();
			}
		}
	}
}
