#include "Axis.h"

Axis::Axis()
{
}

Axis::~Axis()
{
}

void Axis::Draw()
{
	unsigned int red = GetColor(255, 0, 0);
	unsigned int green = GetColor(0, 255, 0);
	unsigned int blue = GetColor(0, 0, 255);
	DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), red); // XŽ²
	DrawCone3D(VECTOR3(100, 0, 0), VECTOR3(80, 0, 0), 10, 20, red, red, 1);
	DrawLine3D(VECTOR3(0, -100, 0), VECTOR3(0, 100, 0), green); // YŽ²
	DrawCone3D(VECTOR3(0, 100, 0), VECTOR3(0, 80, 0), 10, 20, green, green, 1);
	DrawLine3D(VECTOR3(0, 0, -100), VECTOR3(0, 0, 100), blue); // ZŽ²
	DrawCone3D(VECTOR3(0, 0, 100), VECTOR3(0, 0, 80), 10, 20, blue, blue, 1);
}
