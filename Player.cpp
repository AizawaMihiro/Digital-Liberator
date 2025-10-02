#include "Player.h"

Player::Player()
{
	state = IDLE;
	hModel = MV1LoadModel("data/model/Player.mv1");
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
}

void Player::Draw()
{
}

void Player::UpdateIdle()
{
}

void Player::UpdateMove()
{
}

void Player::UpdateJump()
{
}

void Player::UpdateCrouch()
{
}

void Player::UpdateAttack()
{
}

void Player::UpdateDead()
{
}
