#include "Map.h"
#include "Player.h"

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

void Map::Update()
{
	//当たり判定
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player != nullptr)
	{
		for (Block* block : blocks_)
		{
			VECTOR3 playerPos = player->GetTransform().position;
			VECTOR3 blockPos = block->GetTransform().position;
			VECTOR3 playerScale = player->GetTransform().scale;
			if (CheckHitBlock(playerPos,blockPos,playerScale))
			{
				float distX = abs(playerPos.x - blockPos.x);
				float distZ = abs(playerPos.z - blockPos.z);
				float limitX = playerScale.x + BLOCK::SIZE;
				float limitZ = playerScale.z + BLOCK::SIZE;
				float overlapX = limitX - distX;
				float overlapZ = limitZ - distZ;
				Transform playerTransform = player->GetTransform();
				if (overlapX < overlapZ)
				{
					if (playerPos.x < blockPos.x)
					{
						playerTransform.position.x -= overlapX;
					}
					else
					{
						playerTransform.position.x += overlapX;
					}
				}
				else
				{
					if (playerPos.z < blockPos.z)
					{
						playerTransform.position.z -= overlapZ;
					}
					else
					{
						playerTransform.position.z += overlapZ;
					}
				}
				player->SetTransform(playerTransform);
			}
		}
	}
}

void Map::Draw()
{
	if (blocks_.size() < 1)
	{
		for (int r = 0; r < DEF_MAP_SIZE; r++)
		{
			for (int c = 0; c < DEF_MAP_SIZE; c++)
			{
				if (MapData[r][c] == maze::WALL)
				{
					Block* block = new Block(hBlockModel);
					block->SetPosition(VECTOR3{ c * BLOCK::SIZE * 2 ,0.0f,r * BLOCK::SIZE * 2 });
					block->Draw();
					blocks_.push_back(block);
				}
			}
		}
	}
	else
	{
		for (Block* block : blocks_)
		{
			block->Draw();
		}
	}
	
}

bool Map::CheckHitBlock(VECTOR3 playerPos, VECTOR3 blockPos, VECTOR3 playerScale)
{
	float distX = abs(playerPos.x - blockPos.x);
	float distZ = abs(playerPos.z - blockPos.z);
	float limitX = playerScale.x  + BLOCK::SIZE;
	float limitZ = playerScale.z  + BLOCK::SIZE;
	if (distX < limitX && distZ < limitZ)
	{
		return true;
	}
	return false;
}
