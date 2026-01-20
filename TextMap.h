#pragma once
#include "Library/GameObject.h"
#include <vector>
#include <list>

class Player;
class Enemy;

class TextMap : public  GameObject
{
	public:
	TextMap();
	TextMap(std::vector<std::vector<int>> mapData);
	~TextMap();
	void Update() override;
	void Draw() override;
private:
	std::vector<std::vector<int>> MapData;
	Player* player_;
	std::list<Enemy*> enemies_;
};

