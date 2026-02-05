#pragma once
#include "Object3D.h"

namespace GOALPOST{
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
	bool GetGoaled() {
		return TouchGoal;
	}
private:
	bool CheckHitPlayer(VECTOR3 pPos,VECTOR3 pSca);
	bool TouchGoal;
};

