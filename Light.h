#pragma once
#include "DxLib.h"

/// ライト管理クラス Playerクラスの情報を参照する
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
	void CreateAddLight(int type);
private:
	int lightType;
	int hAddLight;
	int addLightType;
};
