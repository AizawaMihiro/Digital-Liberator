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
	//case generator::STICK:
	//	break;
	case generator::DIG:
		maze_ = new MinerMaze(DEF_MAP_SIZE, DEF_MAP_SIZE);
		maze_->Instantiate();
		break;
	//case generator::WALL:
	//	break;
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
	//壁の傍にプレイヤーを生成する位置を決定する処理
	//道でありかつ外壁と隣接しているマスを探し、スタート地点とする
	bool setStart = false;
	for (int i = 0; i < MapData[0].size(); i++)
	{
		if (MapData[1][i] == 0)
		{
			MapData[1][i] = maze::START;
			setStart = true;
			break;
		}
		if (MapData[DEF_MAP_SIZE -2][i] == 0)
		{
			MapData[DEF_MAP_SIZE - 2][i] = maze::START;
			setStart = true;
			break;
		}
	}
	if (!setStart)//生成バグ対策
	{
		for (int i = 0; i < MapData.size(); i++)
		{
			if (MapData[i][1] == 0)
			{
				MapData[i][1] = maze::START;
				setStart = true;
				break;
			}
			if (MapData[i][DEF_MAP_SIZE - 2] == 0)
			{
				MapData[i][DEF_MAP_SIZE - 2] = maze::START;
				setStart = true;
				break;
			}
		}
	}
	//独立した壁（柱）が生成された所の周囲にEnemyが移動する道を生成する処理
	for (int r = 0; r < MapData.size(); r++)
	{
		for (int c = 0; c < MapData[r].size(); c++)
		{
			//柱を探す
			if (MapData[r][c] == maze::WALL)
			{
				int pathCount = 0;
				//周囲8方向を調べる
				for (int y = -1; y <= 1; y++)
				{
					for (int x = -1; x <= 1; x++)
					{
						if (y == 0 && x == 0) {
							continue; // 自分自身はスキップ
						}
						int checkY = r + y;
						int checkX = c + x;
						// 範囲内かチェック
						if (checkY >= 0 && checkY < DEF_MAP_SIZE && checkX >= 0 && checkX < DEF_MAP_SIZE)
						{
							if (MapData[checkY][checkX] == maze::ROAD)
							{
								pathCount++;
							}
						}
					}
				}
				//周囲に壁が無い(柱である)場合、道を生成する
				if (pathCount==8)
				{
					for (int y = -1; y <= 1; y++)
					{
						for (int x = -1; x <= 1; x++)
						{
							if (y == 0 && x == 0) {
								MapData[r][c] = maze::PILLAR;//柱としてマーク
								continue;
							}
							int checkY = r + y;
							int checkX = c + x;
							MapData[checkY][checkX] = maze::PATH;
						}
					}
				}
			}
		}
	}
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
	if (blocks_.size() < 1)//最初の一回だけ迷路の情報から生成する
	{
		//コンパイルエラーC2360回避のために一時的に変数を用意
		//nullptrのものは使用時に初期化する
		Block* block = nullptr;
		Block* pillar = nullptr;
		Enemy* enemy = nullptr;
		Player* player = ObjectManager::FindGameObject<Player>();

		for (int r = 0; r < DEF_MAP_SIZE; r++)
		{
			for (int c = 0; c < DEF_MAP_SIZE; c++)
			{
				switch (MapData[r][c])
				{
				case maze::ROAD:
					break;
				case maze::WALL:
					block = new Block(hBlockModel);
					block->SetPosition(VECTOR3{ c * BLOCK::SIZE * 2 ,0.0f,r * BLOCK::SIZE * 2 });
					block->Draw();
					blocks_.push_back(block);
					break;
				case maze::START:
					if (player != nullptr)
					{
						Transform playerTransform = player->GetTransform();
						playerTransform.position = VECTOR3{ c * BLOCK::SIZE * 2 ,0.0f,(r - 1) * BLOCK::SIZE * 2 };
						player->SetTransform(playerTransform);
					}
					break;
				case maze::GOAL:
					break;
				case maze::PILLAR:
					pillar = new Block(hBlockModel);
					pillar->SetPosition(VECTOR3{ c * BLOCK::SIZE * 2 ,0.0f,r * BLOCK::SIZE * 2 });
					pillar->Draw();
					blocks_.push_back(pillar);
					enemy = new Enemy();
					enemy->SetPosition(VECTOR3{ c * BLOCK::SIZE * 2 ,0.0f,(r - 1) * BLOCK::SIZE * 2 });
					enemies_.push_back(enemy);
					break;
				case maze::PATH:
					break;
				default:
					break;
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
