#pragma once
#include "Object3D.h"

// 座標軸表示用クラス
class Axis :
    public Object3D
{
public:
	Axis();
	~Axis();
	void Draw() override;
};

