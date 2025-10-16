#pragma once
#include "Object3D.h"
enum State
{
	IDLE,
	MOVE,
	HIDE,
	ATTACK,
	DEAD
};

class Player :
    public Object3D
{
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
	void ChangeState(State newState);

private:
	State state;
	void UpdateIdle();
	void UpdateMove();
	void UpdateHide();
	void UpdateAttack();
	void UpdateDead();
};

