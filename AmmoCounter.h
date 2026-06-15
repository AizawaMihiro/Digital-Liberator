#pragma once
namespace AmmoCounter
{
	void AmmoCounter();
	void CountUp();
	void CountDown();
	void Reset();
	int GetCurrentAmmo();

	//GUIに表示するための関数
	void SetDisplayAmmo();
	void UpdateDisplay();
	void DrawDisplay();
};