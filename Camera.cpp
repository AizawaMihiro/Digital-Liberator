#include "Camera.h"
#include "ImGui/imgui.h"
#include "cmath"
#include "Input.h"
#include "Time.h"

namespace {
	float THIRD_CAMERA_DISTANCE = 150.0f;//カメラと注視点の距離
	float FIRST_CAMERA_DISTANCE = 100.0f;
	const float CAMERA_HEIGHT = 30.0f;//カメラの高さ

	const float UPPER_ANGLE = 0.0f;//カメラの上限角度
	const float LOWER_ANGLE = 50.0f;//カメラの下限角度
	const float SINGLE_UPPER_ANGLE = -20.0f;//一人称視点の上限角度
	const float SINGLE_LOWER_ANGLE = 50.0f;//一人称視点の下限角度
	const float RIGHT_ANGLE = 60.0f;//カメラの右限界角度
	const float LEFT_ANGLE = -60.0f;//カメラの左限界角度

	static float transitionTimer = 0.0f;//視点切り替えタイマー
	static bool prevIsThirdPerson = true;//前回の視点モード兼切り替え完了フラグ
	const float CHENGE_DISTANCE = 0.5f;//視点切り替えの距離
	const float SWITCH_SPEED = 2.0f;//切り替えの速さ
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
	int moveX = Input::GetMouseMoveX();
	int moveY = Input::GetMouseMoveY();

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

	//カメラの距離調整
	int wheel = GetMouseWheelRotVol();
	if (wheel!=0)
	{
		THIRD_CAMERA_DISTANCE -= wheel*10;
	}

	CameraSetup(rot,isThirdPerson);

	DebugImGui(rot);
}

void Camera::CameraSetup(VECTOR3 rot, bool viewMode)
{
	// 三人称視点と一人称視点の切り替え
	//三人称と一人称の位置を常に更新しておき、切り替え時にスムーズに移動させる

	//三人称視点の情報
	VECTOR3 ThirdPersonCamPos = targetPosition + VECTOR3(0, CAMERA_HEIGHT - 30.0f, 0) +
		VECTOR3(0, 0, -THIRD_CAMERA_DISTANCE) * MGetRotX(rot.x) * MGetRotY(rot.y);
	VECTOR3 ThirdPersonLookAt = targetPosition + VECTOR3(0, CAMERA_HEIGHT, 0);

	//一人称視点の情報
	VECTOR3 SinglePersonCamPos = targetPosition + 
		VECTOR3(0, CAMERA_HEIGHT, -FIRST_CAMERA_DISTANCE) * MGetRotX(rot.x) * MGetRotY(rot.y);
	VECTOR3 SinglePersonLookAt = SinglePersonCamPos +
		VECTOR3(0, 0.15f, 1.0f) * MGetRotX(rot.x) * MGetRotY(rot.y);

	//スムーズに切り替える処理
	if (prevIsThirdPerson != isThirdPerson)
	{
		transitionTimer += Time::DeltaTime();
		if (isThirdPerson)
		{
			//三人称視点へ切り替え
			SinglePersonCamPos += (ThirdPersonCamPos - SinglePersonCamPos) * SWITCH_SPEED * transitionTimer;
			SinglePersonLookAt += (ThirdPersonLookAt - SinglePersonLookAt) * SWITCH_SPEED * transitionTimer;
			float distance = std::sqrtf(
				(std::pow(SinglePersonCamPos.x - ThirdPersonCamPos.x, 2)) +
				(std::pow(SinglePersonCamPos.y - ThirdPersonCamPos.y, 2)) +
				(std::pow(SinglePersonCamPos.z - ThirdPersonCamPos.z, 2))
			);
			//一定距離まで近づいたら完全に切り替え
			if (distance < CHENGE_DISTANCE)
			{
				prevIsThirdPerson = isThirdPerson;
				transitionTimer = 0.0f;
			}
		}
		else
		{
			//一人称視点へ切り替え
			ThirdPersonCamPos += (SinglePersonCamPos - ThirdPersonCamPos) * SWITCH_SPEED * transitionTimer;
			ThirdPersonLookAt += (SinglePersonLookAt - ThirdPersonLookAt) * SWITCH_SPEED * transitionTimer;
			float distance = std::sqrtf(
				(std::pow(ThirdPersonCamPos.x - SinglePersonCamPos.x, 2)) +
				(std::pow(ThirdPersonCamPos.y - SinglePersonCamPos.y, 2)) +
				(std::pow(ThirdPersonCamPos.z - SinglePersonCamPos.z, 2))
			);
			//一定距離まで近づいたら完全に切り替え
			if (distance < CHENGE_DISTANCE)
			{
				prevIsThirdPerson = isThirdPerson;
				transitionTimer = 0.0f;
			}
		}
	}

	//カメラ位置と注視点の設定
	if (prevIsThirdPerson)
	{
		// 三人称視点の処理
		camPos = ThirdPersonCamPos;
		viewPos = ThirdPersonLookAt;
	}
	else
	{
		// 一人称視点の処理
		camPos = SinglePersonCamPos;
		viewPos = SinglePersonLookAt;
	}
	SetCameraPositionAndTarget_UpVecY(
		camPos,
		viewPos);
}

void Camera::DebugImGui(VECTOR3 rot)
{
	//確認用ImGui
	ImGui::Begin("Camera");
	ImGui::InputFloat("RotX", &rot.x);
	ImGui::InputFloat("RotY", &rot.y);
	ImGui::InputFloat("RotZ", &rot.z);
	ImGui::InputFloat("CameraPosX", &camPos.x);
	ImGui::InputFloat("CameraPosY", &camPos.y);
	ImGui::InputFloat("CameraPosZ", &camPos.z);
	ImGui::InputFloat("TargetPosX", &viewPos.x);
	ImGui::InputFloat("TargetPosY", &viewPos.y);
	ImGui::InputFloat("TargetPosZ", &viewPos.z);
	ImGui::End();
}
