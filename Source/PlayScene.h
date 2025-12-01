#pragma once
#include "../Library/SceneBase.h"

class Player;
class Camera;
class Enemy;
class Map;

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

private:
	Player* player;
	Camera* camera;
	Enemy* enemy;
	Map* map;
};
