#include "Map.h"
#include "Player.h"
#include "Goalpost.h"
#include "TextMap.h"
#include "Camera.h"
#include <assert.h>
#include <queue>

/// <summary>
/// ステージである迷路を生成するクラス
/// 与える因数によって生成方法を変更する
/// </summary>

Map::Map()
	:Map(DIG)
{
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
	hBlockModel_ = MV1LoadModel("Assets/model/shape-cube.mv1");
	assert(hBlockModel_ != -1);
	hPillarModel_ = MV1LoadModel("Assets/model/Blockcolord.mv1");
	assert(hPillarModel_ != -1);
}

Map::~Map()
{
	delete maze_;//ここ足りない
	for (int i = 0; i < blocks_.size(); i++)
	{
		blocks_[i] = nullptr;
	}
	for (int i = 0; i < enemies_.size(); i++)
	{
		enemies_[i] = nullptr;
	}
}

void Map::Instantinate()
{
	MapData = maze_->GetGrid();
	while (!CheckErrorMaze())
	{
		maze_->Instantiate();
		maze_->GetGrid();
	}

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

	//ゴール地点の設定
	//スタート地点から最も遠い経路のマスをゴール地点とする
	int startX = -1;
	int startY = -1;
	for (int r = 0; r < DEF_MAP_SIZE; r++)
	{
		for (int c = 0; c < DEF_MAP_SIZE; c++)
		{
			if (MapData[r][c] == maze::START)
			{
				startX = c;
				startY = r;
				break;
			}
		}
		if (startX != -1 && startY != -1)
		{
			break;
		}
	}
	std::pair<int, int> goalPos = FindMoreDistantPoint(startX, startY);
	MapData[goalPos.first][goalPos.second] = maze::GOAL;


	//迷路データから実際のオブジェクトを生成する処理
	//コンパイルエラーC2360回避のために一時的に変数を用意
		//nullptrのものは使用時に初期化する
	Block* block = nullptr;
	Block* pillar = nullptr;
	Enemy* enemy = nullptr;
	Player* player = ObjectManager::FindGameObject<Player>();
	Goalpost* goalpost = nullptr;

	for (int r = 0; r < DEF_MAP_SIZE; r++)
	{
		for (int c = 0; c < DEF_MAP_SIZE; c++)
		{
			switch (MapData[r][c])
			{
			case maze::ROAD:
				break;
			case maze::WALL:
				block = new Block(hBlockModel_);
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
				goalpost = new Goalpost();
				goalpost->SetPosition(VECTOR3{ c * BLOCK::SIZE * 2 ,GOALPOST::size * 2,r * BLOCK::SIZE * 2 });
				goalpost->Draw();
				break;
			case maze::PILLAR:
				pillar = new Block(hPillarModel_);
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

	textMap_ = new TextMap(MapData);
}

void Map::Update()
{
	//当たり判定
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player)
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
	Camera* camera = ObjectManager::FindGameObject<Camera>();
	if (camera)
	{
		for (Block* block : blocks_)
		{
			VECTOR3 cameraPos = player->GetTransform().position;
			VECTOR3 blockPos = block->GetTransform().position;
			if (CheckHitBlock(cameraPos, blockPos))
			{
				float distX = abs(cameraPos.x - blockPos.x);
				float distZ = abs(cameraPos.z - blockPos.z);
				camera->SetCameraPosition({ cameraPos.x - distX, cameraPos.y, cameraPos.z - distZ });
			}
		}
	}

	//EnemyとPlayerの当たり判定
	gameOverFlag = false;
	for (Enemy* enemy : enemies_) 
	{
		if (!gameOverFlag)
		{
			gameOverFlag = enemy->CheckHitPlayer(player->GetTransform().position, player->GetTransform().scale);
		}
	}
}

void Map::Draw()
{
	for (Block* block : blocks_)
	{
		block->Draw();
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

bool Map::CheckHitBlock(VECTOR3 cameraPos, VECTOR3 blockPos)
{
	//点と四角の当たり判定
	if (blockPos.x <= cameraPos.x && cameraPos.x <= blockPos.x + BLOCK::SIZE)
	{
		if (blockPos.z <= cameraPos.z && cameraPos.z <= blockPos.z + BLOCK::SIZE)
		{
			return true;
		}
	}
	return false;
}

/// <summary>
/// 指定地点から最も遠い場所を探す関数
/// </summary>
/// <returns>std::pair first=Y second=X</returns>
std::pair<int, int> Map::FindMoreDistantPoint(int startX, int startY)
{
	//DFSで全マスを探索し、距離を計測する
	std::pair<int, int> moreDistPoint;
	int maxDist = -1;
	std::vector<std::vector<bool>> visited(DEF_MAP_SIZE, std::vector<bool>(DEF_MAP_SIZE, false));
	std::queue<std::pair<std::pair<int, int>, int>> q;
	q.push({ {startY, startX}, 0 });
	visited[startY][startX] = true;
	while (!q.empty())
	{
		auto current = q.front();
		q.pop();
		int currY = current.first.first;
		int currX = current.first.second;
		int currDist = current.second;
		// 現在の距離が最大距離を更新した場合、座標を保存
		if (currDist > maxDist)
		{
			maxDist = currDist;
			moreDistPoint = { currY, currX };
		}
		// 4方向に移動
		for (int i = 0; i < 4; i++)
		{
			int newY = currY + maze_->dir[i][0];
			int newX = currX + maze_->dir[i][1];
			// 範囲内かつ道であり、未訪問の場合
			if (newY >= 0 && newY < DEF_MAP_SIZE && newX >= 0 && newX < DEF_MAP_SIZE)
			{
				if ((MapData[newY][newX] == maze::ROAD || MapData[newY][newX] == maze::PATH) && !visited[newY][newX])
				{
					visited[newY][newX] = true;
					q.push({ {newY, newX}, currDist + 1 });
				}
			}
		}
	}
	return moreDistPoint;
}

//	MapDataのマップが生成バグで極小だった場合falseを返す
bool Map::CheckErrorMaze()
{
	int countLoad = 0;
	for (int r = 0; r < DEF_MAP_SIZE; r++)
	{
		for (int c = 0; c < DEF_MAP_SIZE; c++)
		{
			switch (MapData[r][c])
			{
			case maze::ROAD:
				countLoad++;
				break;
			case maze::WALL:
				break;
			}
		}
		if (countLoad > 10)
		{
			break;
		}
	}
	if (countLoad < 10)
	{
		return false;
	}
	return true;
}

//ゴールにプレイヤーが触れたかを返す関数
bool Map::GetGameClearFlag()
{
	Goalpost* goal = ObjectManager::FindGameObject<Goalpost>();
	if (goal)
	{
		return goal->GetGoaled();
	}
	return false;
}

//本来は接触ではなくアニメーションを挟んで遷移する
bool Map::GetGameOverFlag()
{
	return gameOverFlag;
}
