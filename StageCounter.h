#pragma once
namespace StageCounter
{
	void StageCounter();
	void CountUp();
	void CountDown();
	void Reset();
	int GetCount();

	//GUIに表示するための関数
	void SetDisplayStage(int stage);//引数は最大ステージ数
};

