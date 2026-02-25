#include "TextMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "global.h"
#include <list>
#include <vector>
#include <string>
#include "ImGui/imgui.h"

namespace Text
{
	int FontSize = 16;
	int FontWidth = 8;
}

//必ずマップデータを渡して生成すること
TextMap::TextMap()
	: TextMap(std::vector<std::vector<int>>())
{
}

TextMap::TextMap(std::vector<std::vector<int>> mapData)
{
	MapData = mapData;
	player_ = FindGameObject<Player>();
	std::list<Enemy*> tempEnemies = ObjectManager::FindGameObjects<Enemy>();
	enemies_.assign(tempEnemies.begin(), tempEnemies.end());
	playerPos_ = std::pair<int, int>(-1, -1);
}

TextMap::~TextMap()
{
}

void TextMap::Update()
{
	//プレイヤーの位置をマップ上の座標に変換して保存する
	if (player_ != nullptr)
	{
		Transform playerTransform = player_->GetTransform();
		int playerMapX = static_cast<int>((playerTransform.position.x + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		int playerMapY = static_cast<int>((playerTransform.position.z + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		playerPos_ = std::pair<int, int>(playerMapX, playerMapY);
	}
	//敵の位置も同様に取得しておく
	if (enemies_.size() != 0) 
	{
		enemyPos_.clear();
		for (auto enemy : enemies_) {
			Transform enemyTransform = enemy->GetTransform();
			int enemyMapX = static_cast<int>((enemyTransform.position.x + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
			int enemyMapY = static_cast<int>((enemyTransform.position.z + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
			enemyPos_.push_back(std::pair<int, int>(enemyMapX, enemyMapY));
		}
	}
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
		int px = playerPos_.first;
		int py = playerPos_.second;
		if (py >= 0 && py < line.size() && px >= 0 && px < line[py].size())
		{
			line[py][px] = "P";
		}
	}
	//敵の位置を反映
	if (enemies_.size() != 0)
	{
		for (auto ePos : enemyPos_) {
			int ex = ePos.first;
			int ey = ePos.second;
			if (ey >= 0 && ey < line.size() && ex >= 0 && ex < line[ey].size())
			{
				line[ey][ex] = "E";
			}
		}
	}
	
	//テキスト描画
	//std::pair<int, int> screenSize = std::pair<int, int>(Global::ScreenWidth, Global::ScreenHeight);
	std::pair<int, int> screenSize = std::pair<int, int>(800, 600); //仮の画面サイズ
	for (int y = 0; y < MapData.size(); y++)
	{
		for (int x = 0; x < MapData[y].size(); x++) {
			DrawString(screenSize.first-(x * Text::FontWidth), 20 + y * Text::FontSize, line[y][x].c_str(), GetColor(255, 255, 255));
		}
	}

	//デバッグ用にグローバル変数を表示
	int Width = Global::ScreenWidth;
	int Height = Global::ScreenHeight;

	ImGui::Begin("Global");
	ImGui::Text("ScreenWidth: %d", Width);
	ImGui::Text("ScreenHeight: %d", Height);
	ImGui::End();
}
