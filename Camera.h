#pragma once
#include "Object3D.h"
class Camera :
    public Object3D
{
public:
	/// <summary>
	/// Playerクラスより早く生成される必要がある
	/// </summary>
	Camera();
	~Camera();
	void Update() override;
	void Draw() override;
	void SetTargetPosition(VECTOR3 pos) { targetPosition = pos; }
private:
	VECTOR3 targetPosition;
	int prevX, prevY;
	bool isThirdPerson;
	int InputTimer;
};

