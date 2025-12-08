#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"

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

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	int moveX = mouseX - prevX;
	prevX = mouseX;
	if (CheckHitKey(KEY_INPUT_LCONTROL))
	{
		transform.rotation.y += moveX * 0.5f * DegToRad;
	}


	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position + VECTOR3(0.0f, 0.0f, 50.0f);
	camera->SetTargetPosition(lookPos);


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
}

void Player::UpdateHide()
{
}

void Player::UpdateAttack()
{
}

void Player::UpdateDead()
{
}

void Player::ChangeState(State newState)
{
	state = newState;
}
