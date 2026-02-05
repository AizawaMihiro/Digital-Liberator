#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"
#include "Time.h"

Player::Player()
	:state(IDLE), cameraMode(THIRD_PERSON)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	transform.position = VZero;
	transform.rotation = VZero;
	VECTOR3 defScale = { (10.0f),(10.0f),(10.0f) };
	transform.scale = defScale;
	camera = FindGameObject<Camera>();
	flameTime = Time::DeltaTime();
}

Player::~Player()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Player::Update()
{
	flameTime = Time::DeltaTime();
	MouseInput();

	if (IsCheckMoveInput())
	{
		ChangeState(MOVE);
	}
	else
	{
		ChangeState(IDLE);
	}

	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		ChangeState(HIDE);
	}
	if (GetMouseInput()&&MOUSE_INPUT_LEFT)
	{
		ChangeState(ATTACK);
	}

	switch (state)
	{
	case MOVE:
		UpdateMove();
		break;
	case HIDE:
		UpdateHide();
		break;
	case ATTACK:
		UpdateAttack();
		break;
	case DEAD:
		UpdateDead();
		break;
	default:
		break;
	}

	if (cameraMode == FIRST_PERSON && state != ATTACK)
	{
		camera->ChangeViewMode(true);
		cameraMode = THIRD_PERSON;
	}

	CameraControl();

	ImGui::Begin("Player");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::InputFloat("RotationY", &transform.rotation.y);
	ImGui::End();
}

void Player::Draw()
{
	Object3D::Draw();
	if (camera->IsThirdPerson())
	{
		MV1DrawModel(hModel);
	}
}

void Player::UpdateMove()
{
	float flameMoveDist = MOVE_SPEED * flameTime * 100;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		moveVec.x += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		moveVec.x -= 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		moveVec.z += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		moveVec.z -= 1.0f;
	}
	transform.position += moveVec.Normalize()* flameMoveDist * MGetRotY(transform.rotation.y);
}

void Player::UpdateHide()
{
	float flameMoveDist = HIDE_SPEED * flameTime * 100;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		moveVec.x += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		moveVec.x -= 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		moveVec.z += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		moveVec.z -= 1.0f;
	}
	transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
}

void Player::UpdateAttack()
{
	UpdateHide();
	if (cameraMode == THIRD_PERSON)
	{
		camera->ChangeViewMode(false);
		cameraMode = FIRST_PERSON;
	}
}

void Player::UpdateDead()
{
	// 死亡処理
}

void Player::ChangeState(State newState)
{
	state = newState;
}

void Player::MouseInput()
{
	moveX = (int)Input::GetMouseMoveX();
	moveY = (int)Input::GetMouseMoveY();
}

bool Player::IsCheckMoveInput()
{
	if (CheckHitKey(KEY_INPUT_W) ||
		CheckHitKey(KEY_INPUT_A) ||
		CheckHitKey(KEY_INPUT_S) ||
		CheckHitKey(KEY_INPUT_D))
	{
		return true;
	}
	return false;
}

void Player::CameraControl()
{
	transform.rotation.y += moveX * 0.5f * DegToRad;

	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position;
	camera->SetTargetPosition(lookPos);
}
