#include "GameOverScene.h"

GameOverScene::GameOverScene()
{
	hImage_ = LoadGraph("Assets/image/GameOver.jpg");
}

GameOverScene::~GameOverScene()
{
	if (hImage_ != -1)
	{
		DeleteGraph(hImage_);
		hImage_ = -1;
	}

}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void GameOverScene::Draw()
{
	DrawGraph(0, 0, hImage_, TRUE);
	DrawString(0, 0, "CLEAR SCENE", GetColor(255, 255, 255));
	//DrawString(100, 300, "GAME OVER", GetColor(255, 0, 0));
	DrawString(550, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
