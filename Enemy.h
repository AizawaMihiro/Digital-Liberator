#pragma once
#include "Object3D.h"
class Enemy :
    public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
private:
	enum State
	{
		PATROL, CHASE, ATTACK, STUN, MAX_STATE
	};
	State state;
	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateStun();
	void ChangeState(State newState);
};

