#include "Camera.h"
#include "ImGui/imgui.h"

namespace {
	const float CAMERA_DISTANCE = 500.0f;//カメラと注視点の距離
	const float CAMERA_HEIGHT = 200.0f;//カメラの高さ
}

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform.rotation.y = 20.0f * DegToRad ;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int moveX = mouseX - prevX;
	int moveY = mouseY - prevY;
	prevX = mouseX;
	prevY = mouseY;

	VECTOR3& rot = transform.rotation;// 回転角度の"参照"
	rot.y += moveX * 0.5f * DegToRad;
	rot.x += moveY * 0.5f * DegToRad;
	// 上下の回転を制限
	if (rot.x > 80.0f * DegToRad) {
		rot.x = 80.0f * DegToRad;
	}
	if (rot.x < -60.0f * DegToRad) {
		rot.x = -60.0f * DegToRad;
	}

	VECTOR3 camPos = VECTOR3(0, 0, -CAMERA_DISTANCE)
		* MGetRotX(rot.x)
		* MGetRotY(rot.y);

	SetCameraPositionAndTarget_UpVecY(
		targetPosition + VECTOR3(0, CAMERA_HEIGHT-50.0f, 0) + camPos,
		targetPosition + VECTOR3(0, CAMERA_HEIGHT, 0));//カメラの位置と注視点の設定

	//確認用ImGui
	VECTOR3 camPosOut = targetPosition + VECTOR3(0, CAMERA_HEIGHT - 50.0f, 0) + camPos;

	ImGui::Begin("Camera");
	ImGui::InputFloat("RotX", &rot.x);
	ImGui::InputFloat("RotY", &rot.y);
	ImGui::InputFloat("RotZ", &rot.z);
	ImGui::InputFloat("CameraPosX", &camPosOut.x);
	ImGui::InputFloat("CameraPosY", &camPosOut.y);
	ImGui::InputFloat("CameraPosZ", &camPosOut.z);
	ImGui::End();
}

void Camera::Draw()
{
}
