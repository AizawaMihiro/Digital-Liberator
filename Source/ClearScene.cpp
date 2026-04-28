#include "ClearScene.h"

ClearScene::ClearScene()
{
	hImage_ = LoadGraph("Assets/image/Clear.jpg");
}

ClearScene::~ClearScene()
{
	if (hImage_ != -1) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	DrawGraph(0, 0, hImage_, TRUE);

	DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	//DrawString(100, 300, "GAME CLEAR", GetColor(255, 255, 0));
	DrawString(550, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
