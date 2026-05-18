#pragma once
#include "Object3D.h"
enum State
{
	IDLE,
	MOVE,
	ATTACK,
	DEAD
};

enum CameraMode
{
	THIRD_PERSON,
	FIRST_PERSON
};

namespace {
	const float MOVE_SPEED = 3.0f;
	const float HIDE_SPEED = 2.0f;
	const float ROT_SPEED = 2.0f;
	const float TARGETING_ROT_SPEED = 0.2f;
	const float ATTACK_RANGE = 200.0f;
	int moveX, moveY;
}

class Camera;
class Object2D;

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
	int gunAmmo_;
	int hViewModel_;
	int hIdleAnim_;
	int hMoveAnim_;
	float flameTime_;	//使用時に*100する
	float animTimer_;	//アニメーションの再生時間を管理する
	int animFrame_;		//アニメーションのフレームを管理する
	int hWalkSound_;
	int hDashSound_;
	bool leftClicked_;
	Transform viewModelTransform;
	State state_;
	Camera* camera_;
	CameraMode cameraMode_;
	Object2D* uiCrosshair_;
	void UpdateMove();
	void UpdateAttack();
	void UpdateDead();
	void ChangeState(State newState);
	void UpdateViewModel();//ViewModelのアニメーション
	void MouseInput();
	bool IsCheckMoveInput();
	void CameraControl();
	void DebugImGui();
};

