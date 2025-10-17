#include "Camera.h"

namespace {
	const float CAMERA_DISTANCE = 50.0f;//ƒJƒƒ‰‚Æ’Ž‹“_‚Ì‹——£
	const float CAMERA_HEIGHT = 20.0f;//ƒJƒƒ‰‚Ì‚‚³
}

Camera::Camera()
{
	GetMousePoint(&prevX, &prevY);
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

	VECTOR3 rot = transform.rotation;
	rot.y += moveX * 0.5f * DegToRad;
	rot.x += moveY * 0.5f * DegToRad;
	// ã‰º‚Ì‰ñ“]‚ð§ŒÀ
	if (rot.x > 80.0f * DegToRad) {
		rot.x = 80.0f * DegToRad;
	}
	if (rot.x < -60.0f * DegToRad) {
		rot.x = -60.0f * DegToRad;
	}


	VECTOR3 camPos = VECTOR3(0, 0, -500.0f)
		* MGetRotX(rot.x)
		* MGetRotY(rot.y);

	SetCameraPositionAndTarget_UpVecY(
		targetPosition + VECTOR3(0, 150, 0) + camPos,
		targetPosition + VECTOR3(0, 200.0f, 0));
}

void Camera::Draw()
{
}
