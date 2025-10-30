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
	DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), red); // XŽ²
	DrawCone3D(VECTOR3(100, 0, 0), VECTOR3(80, 0, 0), 10, 20, red, red, 1);
	DrawLine3D(VECTOR3(0, -100, 0), VECTOR3(0, 100, 0), GetColor(0, 255, 0)); // YŽ²
	DrawCone3D(VECTOR3(0, 100, 0), VECTOR3(0, 80, 0), 10, 20, GetColor(0, 255, 0), GetColor(0, 255, 0), 1);
	DrawLine3D(VECTOR3(0, 0, -100), VECTOR3(0, 0, 100), GetColor(0, 0, 255)); // ZŽ²
	DrawCone3D(VECTOR3(0, 0, 100), VECTOR3(0, 0, 80), 10, 20, GetColor(0, 0, 255), GetColor(0, 0, 255), 1);
}
