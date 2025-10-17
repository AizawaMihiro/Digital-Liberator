#include "Player.h"

Player::Player()
	:state(IDLE)
{
	hModel = MV1LoadModel("Assets/model/Player.mv1");//Ç‹ÇæÉÇÉfÉãÇ™Ç»Ç¢ÇÃÇ≈âº
	if (hModel == -1) {
		// ÉÇÉfÉãÇÃì«Ç›çûÇ›Ç…é∏îs
		printf("Player Model Load Error\n");
	}
	transform.position = VZero;
	transform.scale = VZero;
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
}

void Player::Draw()
{
	if (hModel != -1)
	{
		MV1SetPosition(hModel, transform.position);
		MV1SetRotationXYZ(hModel, transform.rotation);
		MV1SetScale(hModel, transform.scale);
		MV1DrawModel(hModel);
	}
}

void Player::UpdateIdle()
{
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
