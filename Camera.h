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
	void SetTargetPosition(VECTOR3 pos) { targetPosition_ = pos; }
	void SetCameraPosition(VECTOR3 pos) { camPos = pos; }
	void SetCameraRotation(VECTOR3 rot) { transform.rotation = rot; }
	VECTOR3 GetCameraPos() { return camPos; }
	VECTOR3 GetCameraRot() { return transform.rotation; }	
	bool IsThirdPerson() const { return isThirdPerson_; }
	bool ChangeViewMode() { isThirdPerson_ = !isThirdPerson_; return isThirdPerson_; }
	bool ChangeViewMode(bool mode) { isThirdPerson_ = mode; return isThirdPerson_; }
private:
	VECTOR3 targetPosition_;
	int prevX, prevY;
	bool isThirdPerson_;
	int InputTimer_;
	VECTOR3 camPos;//ImGui出力用
	VECTOR3 viewPos;//ImGui出力用

	void CameraSetup(VECTOR3 rot,bool viewMode);
	void DebugImGui(VECTOR3 rot);
};

