#include "TextMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "global.h"
#include <list>
#include <vector>
#include <string>

namespace Text
{
	int FontSize = 16;
	int FontWidth = 8;
}

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
	//テキストデータは右側に表示する

	std::vector<std::vector<std::string>> line = {};
	line.resize(MapData.size());
	for (int y = 0; y < MapData.size(); y++)
	{
		for (int x = 0; x < MapData[y].size(); x++)
		{
			std::string MapChar = " ";
			switch (MapData[y][x])
			{
			case maze::START:
				MapChar = "S";
				break;
			case maze::GOAL:
				MapChar = "G";
				break;
			case maze::WALL:
				MapChar = "#";
				break;
			case maze::ROAD:
				MapChar = ".";
				break;
			case maze::PILLAR:
				MapChar = "0";
				break;
			case maze::PATH:
				MapChar = "+";
				break;
			default:
				break;
			}
			line[y].push_back(MapChar);
		}
	}
	//プレイヤーの位置を反映
	if (player_ != nullptr)
	{
		Transform playerTransform = player_->GetTransform();
		int playerMapX = static_cast<int>(playerTransform.position.x / (BLOCK::SIZE * 2));
		int playerMapY = static_cast<int>(playerTransform.position.z / (BLOCK::SIZE * 2));
		//範囲内チェック
		if (playerMapY >= 0 && playerMapY < line.size() &&
			playerMapX >= 0 && playerMapX < line[playerMapY].size())
		{
			line[playerMapY][playerMapX] = "P";
		}
	}
	
	//テキスト描画
	std::pair<int, int> screenSize = Global::WindowSize();
	//std::pair<int, int> screenSize = std::pair<int, int>(800, 600); //仮の画面サイズ
	for (int y = 0; y < MapData.size(); y++)
	{
		for (int x = 0; x < MapData[y].size(); x++) {
			DrawString(screenSize.first+(-x * Text::FontWidth), 20 + y * Text::FontSize, line[y][x].c_str(), GetColor(255, 255, 255));
		}
	}
}
