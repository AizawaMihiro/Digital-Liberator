#pragma once
#include "Object3D.h"

namespace {
	const int size = 20.0f;
}

class Goalpost : public Object3D
{
public:
	Goalpost();
	~Goalpost();
	void Update() override;
	void Draw() override;
	void SetPosition(VECTOR3 position) {
		transform.position = position;
	}
};

