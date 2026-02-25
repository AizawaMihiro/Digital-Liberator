#pragma once
#include "Object3D.h"

namespace BLOCK
{
	const float HIT_SIZE = 50.0f;//Blockcolordサイズ
	const float VISUAL_SIZE = 1.0f;//shape-cubeサイズ
}

class Block :
    public Object3D
{
public:
	Block();
	Block(int handle);
	~Block();
	void Draw() override;
	void SetPosition(VECTOR3 position) {
		transform.position = position;
	}
private:
};

