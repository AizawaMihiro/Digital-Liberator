#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"

Player::Player()
	:state(IDLE)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	transform.position = VZero;
	transform.rotation = VZero;
	VECTOR3 defScale = { (10.0f),(10.0f),(10.0f) };
	transform.scale = defScale;
	camera = FindGameObject<Camera>();
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
	case IDLE:
		UpdateIdle();
		break;
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

	if (state != ATTACK && !camera->IsThirdPerson())
	{
		camera->ChangeViewMode();
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

void Player::UpdateIdle()
{
	//transform.position.z += 0.25f;
}

void Player::UpdateMove()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform.position.x += MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		transform.position.x -= MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		transform.position.z += MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		transform.position.z -= MOVE_SPEED;
	}
}

void Player::UpdateHide()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform.position.x += HIDE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		transform.position.x -= HIDE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		transform.position.z += HIDE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		transform.position.z -= HIDE_SPEED;
	}
}

void Player::UpdateAttack()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform.position.x += MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		transform.position.x -= MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		transform.position.z += MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		transform.position.z -= MOVE_SPEED;
	}
	if (camera->IsThirdPerson())
	{
		camera->ChangeViewMode();
	}
}

void Player::UpdateDead()
{
}

void Player::ChangeState(State newState)
{
	state = newState;
}

void Player::MouseInput()
{
	GetMousePoint(&mouseX, &mouseY);
	moveX = mouseX - prevX;
	moveY = mouseY - prevY;
	prevX = mouseX;
	prevY = mouseY;
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
	if (CheckHitKey(KEY_INPUT_LCONTROL))
	{
		transform.rotation.y += moveX * 0.5f * DegToRad;
	}

	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position;
	camera->SetTargetPosition(lookPos);
}
