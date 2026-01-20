#include "TextMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include <list>


TextMap::TextMap()
{
}

TextMap::TextMap(std::vector<std::vector<int>> mapData)
{
	MapData = mapData;
	player_ = FindGameObject<Player>();
	std::list<Enemy*> tempEnemies = ObjectManager::FindGameObjects<Enemy>();
	enemies_.assign(tempEnemies.begin(), tempEnemies.end());
}

TextMap::~TextMap()
{
}

void TextMap::Update()
{
	//あとで移動するプレイヤーなどをここで更新する
}

void TextMap::Draw()
{
	//マップデータをもとにテキストのマップを描画する
	for (int y = 0; y < MapData.size(); y++)
	{
		char line[256] = {};
		for (int x = 0; x < MapData[y].size(); x++)
		{
			switch (MapData[y][x])
			{
			case maze::START:
				line[x] = 'S';
				break;
			case maze::GOAL:
				line[x] = 'G';
				break;
			case maze::WALL:
				line[x] = '#';
				break;
			case maze::ROAD:
				line[x] = '.';
				break;
			case maze::PILLAR:
				line[x] = 'P';
				break;
			case maze::PATH:
				line[x] = '+';
				break;
			default:
				break;
			}
		}
		DrawString(0, y * 16, line, GetColor(255, 255, 255),GetColor(0,0,0));
	}
}
