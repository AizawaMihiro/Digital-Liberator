#include "PlayScene.h"
#include "../Player.h"
#include "../Enemy.h"
#include "../Camera.h"
#include "../Axis.h"
#include "../Map.h"
#include "../Light.h"
#include "../StageCounter.h"

namespace
{
	const int CLEAR_STAGE_COUNT = 5;
}

PlayScene::PlayScene()
{
	camera = new Camera();
	player = new Player();
	map = new Map();
	map->Instantinate();
	Axis* axis = new Axis(); // 座標軸表示用 完成後に削除予定

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
		map->DestroyMe();
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
	//ここから条件を満たすまでゲーム部分をループできるようにしていく
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
	light->Update();
	if (map->GetGameClearFlag())
	{
		StageCounter::CountUp();
		if (StageCounter::GetCount()>= CLEAR_STAGE_COUNT)
		{
			SceneManager::ChangeScene("CLEAR");
		}
		else
		{
			SceneManager::ChangeScene("LOAD");
		}
	}
	if (map->GetGameOverFlag())
	{
		SceneManager::ChangeScene("OVER");
	}
}

void PlayScene::Draw()
{
	//DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	//DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
