#include "Object2D.h"
#include "DxLib.h"

Object2D::Object2D()
	:hImage(-1)
{
}

Object2D::~Object2D()
{
}

void Object2D::Draw()
{
	if (hImage != -1)
	{
		DrawGraph(transform.position.x, transform.position.y, hImage,0);
	}
}

void Object2D::SetGraph(std::string filename)
{
	hImage = LoadGraph(filename.c_str(), 0);
}
