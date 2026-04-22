#include "LoadScene.h"

LoadScene::LoadScene()
{
	timer_ = 0;
}

LoadScene::~LoadScene()
{
}

void LoadScene::Update()
{
	timer_ += Time::DeltaTime();
	if (timer_ >= 2)
	{
		SceneManager::ChangeScene("PLAY");
	}
}

void LoadScene::Draw()
{
	DrawString(0, 0, "LOAD SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Waiting...", GetColor(255, 255, 255));
}
