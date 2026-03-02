#pragma once
#include "Object3D.h"
#include "Block.h"
#include <vector>
namespace ENEMY
{
	const float MOVE_SPEED = 0.15f;
	const float CHASE_SPEED = 0.2f;
	const float STUN_TIME = 10.0f;
	const float CHASE_RANGE = 150.0f;
}

class Enemy :
    public Object3D
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	void SetPosition(VECTOR3 pos);
	bool CheckHitPlayer(VECTOR3 pPos, VECTOR3 pSca);
	void SetStateStun();
private:
	enum State
	{
		PATROL, CHASE, RETURN, STUN, MAX_STATE
	};
	State state_;
	VECTOR3 homePosition_;
	std::vector<VECTOR3> patrolPoints_;
	int currentPatrolIndex;
	void UpdatePatrol();
	void UpdateChase();
	void UpdateReturn();
	void UpdateStun();
	void ChangeState(State newState);
	float flameTime;	//使用時に*100する
	float stunTimer;	//スタン状態の残り時間
	bool returndFlag;	//patrolPointsに戻ったかどうか
};

