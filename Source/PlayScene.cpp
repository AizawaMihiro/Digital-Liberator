#include "PlayScene.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Camera.h"
#include "../Axis.h"
#include "../Map.h"
#include "../Light.h"

PlayScene::PlayScene()
{
	camera = new Camera();
	player = new Player();
	enemy = new Enemy();
	map = new Map();
	map->Instantinate();
	map->Draw();
	Axis* axis = new Axis(); // À•WŽ²•\Ž¦—p

	light = new Light();
	light->ChangeLight(DX_LIGHTTYPE_D3DLIGHT_POINT);

	camera->SetTargetPosition(player->GetTransform().position);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	light->Update();
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
