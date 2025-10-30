#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"

Player::Player()
	:state(IDLE)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//‚Ü‚¾ƒ‚ƒfƒ‹‚ª‚È‚¢‚Ì‚Å‰¼
	transform.position = VZero;
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
	ImGui::Begin("Player");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::End();

	camera->SetTargetPosition(transform.position);
}

void Player::Draw()
{
	Object3D::Draw();
	MV1DrawModel(hModel);
}

void Player::UpdateIdle()
{
	transform.position.z += 0.25f;
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
