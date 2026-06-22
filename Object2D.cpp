#include "Object2D.h"
#include "DxLib.h"

Object2D::Object2D()
	:hImage(-1), drawFlag(true), transFlag(0)
{
	GameObject::SetDrawOrder(0);//2Dオブジェクトは3Dオブジェクトより後に描画されるようにする
	extendWidth = 0;
	extendHeight = 0;
}

Object2D::~Object2D()
{
}

void Object2D::Draw()
{
	if (hImage != -1)
	{
		if (drawFlag)
		{
			if (extendWidth != 0 && extendHeight != 0)
			{
				DrawExtendGraph(transform.position.x, transform.position.y, transform.position.x + extendWidth, transform.position.y + extendHeight, hImage, transFlag);
			}
			else
			{
				DrawGraph(transform.position.x, transform.position.y, hImage, transFlag);
			}
		}
	}
}

void Object2D::SetGraph(std::string filename)
{
	hImage = LoadGraph(filename.c_str(), 0);
}
