#include "TitleScene.h"
#include "Time.h"
#include "../StageCounter.h"
#include <iostream>

namespace
{
	const int SOUND_VOLUME = 75;//255が最大 30%ほどの音量に設定
}

TitleScene::TitleScene()
{
	StageCounter::Reset();
	hImage_ = LoadGraph("Assets/image/Title.jpg");
	textTimer_ = 0;
	startTimer_ = 0;
	PressStartKey_ = false;

	hBGM_ = LoadSoundMem("Assets/sound/Funky_Crazy.mp3");
	hStartSound_ = LoadSoundMem("Assets/sound/se/start.mp3");

	PlaySoundMem(hBGM_, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(SOUND_VOLUME, hBGM_);
}

TitleScene::~TitleScene()
{
	if (hImage_ != -1) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
	DeleteSoundMem(hBGM_);
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)&&!PressStartKey_) {
		PlaySoundMem(hStartSound_, DX_PLAYTYPE_BACK);
		PressStartKey_ = true;
		textTimer_ = 1.5f;//スタートキーを押したときにテキストを表示するためにタイマーをリセットする
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}

	//スタートキーを押してから1秒後にシーンを切り替える
	if (PressStartKey_)
	{
		startTimer_ += Time::DeltaTime();
		if (startTimer_ > 1.0f)
		{
			SceneManager::ChangeScene("PLAY");
		}
	}
	else {
		textTimer_ += Time::DeltaTime();
		if (textTimer_ > 3.0f)
		{
			textTimer_ -= 3.0f;
		}
	}
}


void TitleScene::Draw()
{
	DrawGraph(0, 0, hImage_, TRUE);

	//extern const char* Version();
	//std::string LimitTime = std::to_string(Time::DeltaTime());
	//DrawString(0, 20, Version(), GetColor(255,255,255));
	//DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	if (textTimer_ >= 1.5f)
	{
		DrawString(550, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
	}
	//DrawString(0,40,LimitTime.c_str(), GetColor(255, 255, 255));
}
