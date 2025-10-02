#pragma once
#include "Object3D.h"

class Player :
    public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
private:
	enum State
	{
		IDLE, MOVE, JUMP, CROUCH, ATTACK, DEAD, MAX_STATE
	};
	State state;
	void UpdateIdle();
	void UpdateMove();
	void UpdateJump();
	void UpdateCrouch();
	void UpdateAttack();
	void UpdateDead();
	void ChangeState(State newState);
};

