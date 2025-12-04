#pragma once
#include "Object3D.h"

namespace BLOCK
{
	const float SIZE = 50.0f;
}

class Block :
    public Object3D
{
public:
	Block();
	~Block();
	void Draw() override;
	void SetPosition(VECTOR3 position) {
		transform.position = position;
	}
private:
};

