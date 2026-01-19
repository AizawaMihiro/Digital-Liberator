#pragma once
#include "Object3D.h"
#include "Block.h"
#include <vector>

class Enemy :
    public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	void SetPosition(VECTOR3 pos);
private:
	enum State
	{
		PATROL, CHASE, ATTACK, STUN, MAX_STATE
	};
	State state;
	VECTOR3 homePosition;
	std::vector<VECTOR3> patrolPoints;
	int currentPatrolIndex;
	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateStun();
	void ChangeState(State newState);
};

