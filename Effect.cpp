#include "Effect.h"
#include "Library/Time.h"
/// <summary>
///		呼び出し時の指定した秒数で自動消滅するクラス
/// </summary>

//基本的に変数ありで呼び出す
Effect::Effect()
	:hEffect(-1),LimitTime(0)
{
}

Effect::Effect(int handle, int time)
{
	hEffect = handle;
	LimitTime = time;
}

Effect::~Effect()
{
}

void Effect::Update()
{
	nowTime += Time::DeltaTime();
	if (nowTime>LimitTime)
	{
		this->DestroyMe();
	}
}

void Effect::Draw()
{

}
