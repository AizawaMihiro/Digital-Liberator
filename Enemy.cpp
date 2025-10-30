#include "Enemy.h"
#include "ImGui/imgui.h"

Enemy::Enemy()
	:state(PATROL)
{
	hModel = MV1LoadModel("Assets/model/enemy01.mv1");//‚Ü‚¾ƒ‚ƒfƒ‹‚ª‚È‚¢‚Ì‚Å‰¼
	assert(hModel != -1);
	VECTOR3 defPos = { (0.0f),(0.0f),(50.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { (0.5f),(0.25f),(0.5f) };
	transform.scale = defScale;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	switch (state)
	{
	case PATROL:
		UpdatePatrol();
		break;
	case CHASE:
		UpdateChase();
		break;
	case ATTACK:
		UpdateAttack();
		break;
	case STUN:
		UpdateStun();
		break;
	default:
		break;
	}

	ImGui::Begin("Enemy");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::End();
}

void Enemy::Draw()
{
	Object3D::Draw();
	MV1DrawModel(hModel);
}

void Enemy::UpdatePatrol()
{
	static float movex = 0.1f;
	transform.position.x += movex;
	if (transform.position.x > 50.0f || transform.position.x < -50.0f) {
		movex = -movex;
	}
}

void Enemy::UpdateChase()
{
}

void Enemy::UpdateAttack()
{
}

void Enemy::UpdateStun()
{
}

void Enemy::ChangeState(State newState)
{
	state = newState;
}
