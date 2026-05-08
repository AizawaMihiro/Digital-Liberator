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
	VECTOR3 GetPosition() { return transform.position; }

private:
	int hViewModel_;
	int hIdleAnim_;
	int hMoveAnim_;
	Transform viewModelTransform;
	State state_;
	Camera* camera;
	CameraMode cameraMode;
	void UpdateMove();
	void UpdateHide();
	void UpdateAttack();
	void UpdateDead();
	void ChangeState(State newState);
	void UpdateViewModel();//ViewModelのアニメーション
	void MouseInput();
	bool IsCheckMoveInput();
	void CameraControl();
	float flameTime;	//使用時に*100する
	float animTimer;	//アニメーションの再生時間を管理する
	int animFrame;		//アニメーションのフレームを管理する
};

