#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"

namespace {
	float CAMERA_DISTANCE = 250.0f;//カメラと注視点の距離
	const float CAMERA_HEIGHT = 50.0f;//カメラの高さ
	const float UPPER_ANGLE = 0.0f;//カメラの上限角度
	const float LOWER_ANGLE = 50.0f;//カメラの下限角度
	const float SINGLE_UPPER_ANGLE = -20.0f;//一人称視点の上限角度
	const float SINGLE_LOWER_ANGLE = 50.0f;//一人称視点の下限角度
	const float RIGHT_ANGLE = 60.0f;//カメラの右限界角度
	const float LEFT_ANGLE = -60.0f;//カメラの左限界角度
}

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
	transform.position = VECTOR3(0, 0, 0);
	transform.rotation = VECTOR3(20.0f * DegToRad, 0, 0);
	isThirdPerson = true;
	InputTimer = 0;
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
	if (isThirdPerson)
	{
		if (rot.x < UPPER_ANGLE * DegToRad) {
			rot.x = UPPER_ANGLE * DegToRad;
		}
		if (rot.x > LOWER_ANGLE * DegToRad) {
			rot.x = LOWER_ANGLE * DegToRad;
		}
	}
	else
	{
		if (rot.x < SINGLE_UPPER_ANGLE * DegToRad) {
			rot.x = SINGLE_UPPER_ANGLE * DegToRad;
		}
		if (rot.x > SINGLE_LOWER_ANGLE * DegToRad) {
			rot.x = SINGLE_LOWER_ANGLE * DegToRad;
		}
	}

	// 左右の回転を制限
	//if (rot.y > RIGHT_ANGLE * DegToRad) {
	//	rot.y = RIGHT_ANGLE * DegToRad;
	//}
	//if (rot.y < LEFT_ANGLE * DegToRad) {
	//	rot.y = LEFT_ANGLE * DegToRad;
	//}

	// カメラ位置操作
	// デバッグ用
	//static float camXmove = 0.0f;
	//static float camZmove = 0.0f;
	//if (CheckHitKey(KEY_INPUT_LEFT))
	//{
	//	camXmove -= 1.0f;
	//}
	//if (CheckHitKey(KEY_INPUT_RIGHT))
	//{
	//	camXmove += 1.0f;
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	camZmove += 1.0f;
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	camZmove -= 1.0f;
	//}
	//if (CheckHitKey(KEY_INPUT_0))
	//{
	//	// リセット
	//	camXmove = 0.0f;
	//	camZmove = 0.0f;
	//}
	//targetPosition.x += camXmove;
	//targetPosition.z += camZmove;

	//カメラの距離調整
	int wheel = GetMouseWheelRotVol();
	if (wheel!=0)
	{
		CAMERA_DISTANCE -= wheel*10;
	}

	VECTOR3 camPos;

	// 三人称視点と一人称視点の切り替え
	//if (Input::IsKeyDown(KEY_INPUT_V))
	//{
	//	isThirdPerson = !isThirdPerson;
	//}
	if (isThirdPerson)
	{
		camPos = VECTOR3(0, 0, -CAMERA_DISTANCE)
			* MGetRotX(rot.x)
			* MGetRotY(rot.y);

		SetCameraPositionAndTarget_UpVecY(
			targetPosition + VECTOR3(0, CAMERA_HEIGHT - 50.0f, 0) + camPos,
			targetPosition + VECTOR3(0, CAMERA_HEIGHT, 0));//カメラの位置と注視点の設定
	}
	else
	{
		// 一人称視点の処理
		camPos = targetPosition + VECTOR3(0, CAMERA_HEIGHT, -100);

		VECTOR3 lookAt = VECTOR3(0, 0, 1)
			* MGetRotX(rot.x)
			* MGetRotY(rot.y);

		SetCameraPositionAndTarget_UpVecY(
			camPos,
			camPos + lookAt);//カメラの位置と注視点の設定
	}


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
