#include "AmmoCounter.h"
#include "Object2D.h"
#include <string>

namespace
{
    const int LIMIT_AMMO = 5;
	static int currentAmmo = 0;
	static bool isInitialized = false;

	const int DISPLAY_NUMBER = 5;
	Object2D* ammoDisplay[DISPLAY_NUMBER] = { nullptr };
}

void AmmoCounter::AmmoCounter()
{
	currentAmmo = LIMIT_AMMO;
}

void AmmoCounter::CountUp()
{
	currentAmmo++;
	AmmoCounter::UpdateDisplay();
}

void AmmoCounter::CountDown()
{	
	currentAmmo--;
	AmmoCounter::UpdateDisplay();
}

void AmmoCounter::Reset()
{
	currentAmmo = LIMIT_AMMO;
	AmmoCounter::UpdateDisplay();
}

int AmmoCounter::GetCurrentAmmo()
{
    return currentAmmo;
}


//フォーマットは「現在の弾数/最大弾数」
void AmmoCounter::SetDisplayAmmo()
{
	if (!isInitialized)
	{
		//弾数表示用のオブジェクトを生成する
		for (int i=0; i< DISPLAY_NUMBER; i++)
		{
			ammoDisplay[i] = new Object2D();
			ammoDisplay[i]->SetDrawFlag(false);//最初は非表示にしておく
			ammoDisplay[i]->SetTransFlag(1);
			Transform displayTransform = ammoDisplay[i]->GetTransform();
			VECTOR3 displayPos = { 20.0f + i * 80.0f, 500.0f, 0.0f };//表示位置を調整
			displayTransform.position = displayPos;
			ammoDisplay[i]->SetTransform(displayTransform);

			int displayNumber;
			std::string graphPath;
			switch (i)
			{
			case 0:
			case 3:
				displayNumber = LIMIT_AMMO / 10; // 最大弾数の10の位を表示
				graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
				break;
			case 1:
			case 4:
				displayNumber = LIMIT_AMMO % 10; // 最大弾数の1の位を表示
				graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
				break;
			case 2:
				graphPath = "Assets/image/number/7segu-slash-C.png"; // スラッシュを表示
				break;
			default:
				break;
			}
			ammoDisplay[i]->SetGraph(graphPath.c_str());
		}
		isInitialized = true;
	}
}

void AmmoCounter::UpdateDisplay()
{
	//現在の弾数を表示するために、ammoDisplayのグラフィックを更新する
	for (int i = 0; i < 2; i++)
	{
		int displayNumber;
		std::string graphPath;
		switch (i)
		{
		case 0:
			displayNumber = currentAmmo / 10; // 10の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		case 1:
			displayNumber = currentAmmo % 10; // 1の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		default:
			break;
		}
		ammoDisplay[i]->SetGraph(graphPath.c_str());
	}
}

void AmmoCounter::DrawDisplay()
{
	for (int i = 0; i < DISPLAY_NUMBER; i++)
	{
		ammoDisplay[i]->SetDrawFlag(true);
	}
}
