#include "TitleScene.h"
#include "Time.h"
#include "../StageCounter.h"
#include <iostream>

TitleScene::TitleScene()
{
	StageCounter::Reset();
	hImage_ = LoadGraph("Assets/image/Title.jpg");
	timer_ = 0;
}

TitleScene::~TitleScene()
{
	if (hImage_ != -1) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
	timer_ += Time::DeltaTime();
	if (timer_ > 3.0f)
	{
		timer_ -= 3.0f;
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, hImage_, TRUE);

	extern const char* Version();
	std::string LimitTime = std::to_string(Time::DeltaTime());
	DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	if (timer_ >= 1.5f)
	{
		DrawString(550, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
	}
	DrawString(0,40,LimitTime.c_str(), GetColor(255, 255, 255));
}
