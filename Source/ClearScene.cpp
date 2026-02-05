#include "ClearScene.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	DrawString(100, 300, "GAME CLEAR", GetColor(255, 255, 0));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
