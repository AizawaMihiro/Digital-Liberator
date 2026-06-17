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
	const int CLEAR_STAGE_COUNT = 3;
	const int SOUND_VOLUME = 75;//255が最大 30%ほどの音量に設定
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

	hBGM_ = LoadSoundMem("Assets/sound/Escape.mp3");

	PlaySoundMem(hBGM_, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(SOUND_VOLUME, hBGM_);

	StageCounter::SetDisplayStage(CLEAR_STAGE_COUNT);
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
	DeleteSoundMem(hBGM_);
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
