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

enum CameraMode
{
	THIRD_PERSON,
	FIRST_PERSON
};

namespace {
	const float MOVE_SPEED = 4.0f;
	const float HIDE_SPEED = 2.0f;
	int moveX, moveY;
}

class Camera;

class Player :
    public Object3D
{
public:
	/// <summary>
	/// Cameraクラスより後に生成される必要がある
	/// </summary>
	Player();
	~Player();
	void Update() override;
	void Draw() override;
	void ChangeState(State newState);

private:
	State state;
	CameraMode cameraMode;
	void UpdateIdle();
	void UpdateMove();
	void UpdateHide();
	void UpdateAttack();
	void UpdateDead();
	Camera* camera;
	void MouseInput();
	bool IsCheckMoveInput();
	void CameraControl();
	float flameTime;	//使用時に*100する
};

