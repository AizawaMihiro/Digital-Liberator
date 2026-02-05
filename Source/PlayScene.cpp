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
	map = new Map();
	map->Instantinate();
	map->Draw();
	Axis* axis = new Axis(); // À•WŽ²•\Ž¦—p Š®¬Œã‚Éíœ—\’è

	light = new Light();
	light->ChangeLight(DX_LIGHTTYPE_DIRECTIONAL);
	light->CreateAddLight(DX_LIGHTTYPE_POINT);

	camera->SetTargetPosition(player->GetTransform().position);
}

PlayScene::~PlayScene()
{
	if (player != nullptr)
	{
		player->DestroyMe();
		player = nullptr;
	}
	if (camera != nullptr)
	{
		camera->DestroyMe();
		camera = nullptr;
	}
	if (map != nullptr)
	{
		delete map;
		map = nullptr;
	}
	if (light != nullptr)
	{
		delete light;
		light = nullptr;
	}
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	light->Update();
	map->Update();
	if (map->GetGameEndFlag())
	{
		SceneManager::ChangeScene("CLEAR");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
