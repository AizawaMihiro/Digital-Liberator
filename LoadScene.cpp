#include "LoadScene.h"
#include <stdlib.h>
#include <time.h>

LoadScene::LoadScene()
{
	srand((unsigned int)time(NULL));
	int randomNum = rand() % 2;
	if (randomNum==0)
	{
		hImage_ = LoadGraph("Assets/image/Load hide.jpg");
	}
	else
	{
		hImage_ = LoadGraph("Assets/image/Load check.jpg");
	}
	
	timer_ = 0;
}

LoadScene::~LoadScene()
{
	if (hImage_ != -1) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
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
	DrawGraph(0, 0, hImage_, TRUE);
	//DrawString(0, 0, "LOAD SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Loading...", GetColor(255, 255, 255));
}
