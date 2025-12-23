#pragma once
#include "DxLib.h"

class Light
{
public:
	Light();
	~Light();
	/// <summary>
	/// ライトの種類を変更する
	/// </summary>
	/// <param name="type">DX_LIGHTTYPE_~で表記する</param>
	void ChangeLight(int type);
	int DefaultLight(bool flag);
	void Update();
private:
	int hLight;
};
