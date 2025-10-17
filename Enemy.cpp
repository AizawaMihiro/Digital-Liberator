#include "Enemy.h"

Enemy::Enemy()
	:state(PATROL)
{
	hModel = MV1LoadModel("Assets/model/Enemy.mv1");//Ç‹ÇæÉÇÉfÉãÇ™Ç»Ç¢ÇÃÇ≈âº
	if (hModel == -1) {
		// ÉÇÉfÉãÇÃì«Ç›çûÇ›Ç…é∏îs
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
	if (hModel != -1)
	{
		MV1SetPosition(hModel, transform.position);
		MV1SetRotationXYZ(hModel, transform.rotation);
		MV1SetScale(hModel, transform.scale);
		MV1DrawModel(hModel);
	}
}

void Enemy::UpdatePatrol()
{
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
