#pragma once
#include "Object3D.h"
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

