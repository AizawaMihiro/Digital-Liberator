#include "AmmoCounter.h"
#include "Object2D.h"
#include <string>
#include "Source/Screen.h"

namespace AmmoCounter
{
    const int LIMIT_AMMO = 5;
	static int currentAmmo = 0;
	const int BG_WIDTH = 500;
	const int BG_HEIGHT = 250;
	const int BG_POS_X = 0;
	const int BG_POS_Y = Screen::HEIGHT - BG_HEIGHT;
	const int DISPLAY_WIDTH = 80;
	const int DISPLAY_HEIGHT = 150 + 30;//マージン込み
	const int TITLE_X = 20;
	const int TITLE_Y = Screen::HEIGHT - 240;

	const int DISPLAY_NUMBER = 5;
	Object2D* ammoDisplay[DISPLAY_NUMBER] = { nullptr };
	Object2D* ammoTitle = nullptr;
	Object2D* ammoBG = nullptr;
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

void AmmoCounter::CountReset()
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
	//背景用のオブジェクトを生成する
	ammoBG = new Object2D();
	ammoBG->SetDrawFlag(true);
	ammoBG->SetTransFlag(1);
	ammoBG->SetExtend(BG_WIDTH, BG_HEIGHT); // 背景の拡大描画サイズを設定
	Transform BGTransform = ammoBG->GetTransform();
	VECTOR3 BGPos = { BG_POS_X, BG_POS_Y, 0.0f }; // 背景の表示位置を調整
	BGTransform.position = BGPos;
	ammoBG->SetTransform(BGTransform);
	ammoBG->SetGraph("Assets/image/BackGround.png"); // 背景画像を設定

	//弾数表示用のオブジェクトを生成する
	for (int i = 0; i < DISPLAY_NUMBER; i++)
	{
		ammoDisplay[i] = new Object2D();
		ammoDisplay[i]->SetDrawFlag(false);//最初は非表示にしておく
		ammoDisplay[i]->SetTransFlag(1);
		Transform displayTransform = ammoDisplay[i]->GetTransform();
		VECTOR3 displayPos = { (float)(i * DISPLAY_WIDTH), Screen::HEIGHT - DISPLAY_HEIGHT, 0.0f };//表示位置を調整
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
	ammoTitle = new Object2D();
	ammoTitle->SetDrawFlag(true);
	ammoTitle->SetTransFlag(1);
	Transform titleTransform = ammoTitle->GetTransform();
	VECTOR3 titlePos = { TITLE_X, TITLE_Y, 0.0f }; // タイトルの表示位置を調整
	titleTransform.position = titlePos;
	ammoTitle->SetTransform(titleTransform);
	ammoTitle->SetGraph("Assets/image/Ammo.png"); // タイトル画像を設定
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
