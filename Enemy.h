#pragma once
#include "Object3D.h"
#include "Block.h"
#include <vector>
namespace ENEMY
{
	const float MOVE_SPEED = 0.15f;
	const float CHASE_SPEED = 0.3f;
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
	void UpdateCollision();
private:
	enum State
	{
		PATROL, CHASE, RETURN, STUN, MAX_STATE
	};
	enum Anim
	{
		STOP, IDLE, WALK, MAX_ANIM
	};
	State state_;
	VECTOR3 homePosition_;
	std::vector<VECTOR3> patrolPoints_;
	int currentPatrolIndex;
	int hViewModel_;
	Transform viewModelTransform;
	int animFrame_;
	float animTimer_;
	float flameTime_;	//使用時に*100する
	float stunTimer_;	//スタン状態の残り時間
	bool returndFlag_;	//patrolPointsに戻ったかどうか
	int hCheckSound_;
	int hStunSound_;
	void UpdatePatrol();
	void UpdateChase();
	void UpdateReturn();
	void UpdateStun();
	void UpdateViewModel();
	void ChangeState(State newState);
	void DebugImGui();
};

