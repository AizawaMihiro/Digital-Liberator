#include "StageCounter.h"
#include "Object2D.h"

namespace 
{
	int count_;

	const int DISPLAY_NUMBER = 5;
	Object2D* stageDisplay[DISPLAY_NUMBER] = {nullptr};
	Object2D* stageTitle = nullptr;
}

void StageCounter::StageCounter()
{
	count_ = 0;
}


void StageCounter::CountUp()
{
	count_++;
}

void StageCounter::CountDown()
{
	count_--;
}

void StageCounter::Reset()
{
	count_ = 0;
}

int StageCounter::GetCount()
{
	return count_;
}

//フォーマットは「現在のステージ数/最大ステージ数」
void StageCounter::SetDisplayStage(int stage)
{
	for (int i = 0; i < DISPLAY_NUMBER; i++)
	{
		stageDisplay[i] = new Object2D();
		stageDisplay[i]->SetDrawFlag(true);
		stageDisplay[i]->SetTransFlag(1);
		Transform displayTransform = stageDisplay[i]->GetTransform();
		VECTOR3 displayPos = { i * 80.0f, 60.0f, 0.0f };//表示位置を調整
		displayTransform.position = displayPos;
		stageDisplay[i]->SetTransform(displayTransform);
		int displayNumber;
		std::string graphPath;
		int displaycount = count_ + 1;//ステージ数は0から始まるので、表示する数は+1する
		switch (i)
		{
		case 0:
			displayNumber = displaycount / 10; // 10の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		case 1:
			displayNumber = displaycount % 10; // 1の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		case 2:
			graphPath = "Assets/image/number/7segu-slash-C.png"; // スラッシュを表示
			break;
		case 3:
			displayNumber = stage / 10; // 最大ステージの10の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		case 4:
			displayNumber = stage % 10; // 最大ステージの1の位を表示
			graphPath = "Assets/image/number/7segu-" + std::to_string(displayNumber) + "-C.png";
			break;
		default:
			break;
		}
		stageDisplay[i]->SetGraph(graphPath.c_str());
	}
	stageTitle = new Object2D();
	stageTitle->SetDrawFlag(true);
	stageTitle->SetTransFlag(1);
	Transform titleTransform = stageTitle->GetTransform();
	VECTOR3 titlePos = { -20.0f, -20.0f, 0.0f }; // タイトルの表示位置を調整
	titleTransform.position = titlePos;
	stageTitle->SetTransform(titleTransform);
	stageTitle->SetGraph("Assets/image/Floor.png"); // タイトル画像を設定
}