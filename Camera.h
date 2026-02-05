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
	void SetTargetPosition(VECTOR3 pos) { targetPosition = pos; }
	void SetCameraPosition(VECTOR3 pos) { camPos = pos; }
	VECTOR3 GetCameraPos() { return camPos; }
	bool IsThirdPerson() const { return isThirdPerson; }
	bool ChangeViewMode() { isThirdPerson = !isThirdPerson; return isThirdPerson; }
	bool ChangeViewMode(bool mode) { isThirdPerson = mode; return isThirdPerson; }
private:
	VECTOR3 targetPosition;
	int prevX, prevY;
	bool isThirdPerson;
	int InputTimer;
	VECTOR3 camPos;//ImGui出力用
	VECTOR3 viewPos;//ImGui出力用

	void CameraSetup(VECTOR3 rot,bool viewMode);
	void DebugImGui(VECTOR3 rot);
};

