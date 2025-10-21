#include "Enemy.h"

Enemy::Enemy()
	:state(PATROL)
{
	hModel = MV1LoadModel("Assets/model/enemy01.mv1");//‚Ü‚¾ƒ‚ƒfƒ‹‚ª‚È‚¢‚Ì‚Å‰¼
	if (hModel == -1) {
		// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý‚ÉŽ¸”s
		printf("Enemy Model Load Error\n");
	}
	transform.position = VZero;
	transform.scale = VZero;
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
}

void Enemy::Draw()
{
	Object3D::Draw();
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
