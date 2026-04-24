#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"
#include "Time.h"

Player::Player()
	:state_(IDLE), cameraMode(THIRD_PERSON)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	transform.position = VZero;
	transform.rotation = VZero;
	VECTOR3 defScale = { (10.0f),(10.0f),(10.0f) };
	transform.scale = defScale;

	hViewModel = MV1LoadModel("Assets/model/3Dchara man.mv1");
	viewModelTransform.position = VZero;
	VECTOR3 viewDefRot = { (0.0f),(180.0f * DegToRad),(0.0f) };
	viewModelTransform.rotation = viewDefRot;
	VECTOR3 viewDefScale = { (0.2f),(0.2f),(0.2f) };
	viewModelTransform.scale = viewDefScale;

	camera = FindGameObject<Camera>();
	flameTime = Time::DeltaTime();
}

Player::~Player()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
	if (hViewModel != -1)
	{
		MV1DeleteModel(hViewModel);
		hViewModel = -1;
	}
}

void Player::Update()
{
	flameTime = Time::DeltaTime();
	MouseInput();

	if (IsCheckMoveInput())
	{
		ChangeState(MOVE);
	}
	else
	{
		ChangeState(IDLE);
	}

	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		ChangeState(HIDE);
	}
	if (GetMouseInput()&&MOUSE_INPUT_LEFT)
	{
		ChangeState(ATTACK);
	}

	switch (state_)
	{
	case MOVE:
		UpdateMove();
		break;
	case HIDE:
		UpdateHide();
		break;
	case ATTACK:
		UpdateAttack();
		break;
	case DEAD:
		UpdateDead();
		break;
	default:
		break;
	}

	viewModelTransform.position = transform.position;
	viewModelTransform.rotation = transform.rotation + VECTOR3(0.0f, 180.0f * DegToRad, 0.0f);

	if (cameraMode == FIRST_PERSON && state_ != ATTACK)
	{
		camera->ChangeViewMode(true);
		cameraMode = THIRD_PERSON;
	}

	CameraControl();

	ImGui::Begin("Player");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::InputFloat("RotationY", &transform.rotation.y);
	ImGui::InputFloat("ViewPosX", &viewModelTransform.position.x);
	ImGui::InputFloat("ViewPosY", &viewModelTransform.position.y);
	ImGui::InputFloat("ViewPosZ", &viewModelTransform.position.z);
	ImGui::InputFloat("ViewRotY", &viewModelTransform.rotation.y);
	ImGui::End();
}

void Player::Draw()
{
	// ビューモデルの描画
	if (hViewModel != -1)
	{
		const MATRIX& local = viewModelTransform.MakeLocalMatrix();
		if (parent != nullptr) {
			const MATRIX& parentLocal = parent->GetTransform().GetLocalMatrix();
			MATRIX world = local * parentLocal;
			MV1SetMatrix(hViewModel, world);
		}
		else {
			MV1SetMatrix(hViewModel, local);
		}
	}

	if (camera->IsThirdPerson())
	{
		MV1DrawModel(hViewModel);
	}
}

void Player::UpdateMove()
{
	float flameMoveDist = MOVE_SPEED * flameTime * 100;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		moveVec.x += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		moveVec.x -= 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		moveVec.z += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		moveVec.z -= 1.0f;
	}
	transform.position += moveVec.Normalize()* flameMoveDist * MGetRotY(transform.rotation.y);
}

void Player::UpdateHide()
{
	//しゃがみ移動を行う
	//やや遅くなる
	float flameMoveDist = HIDE_SPEED * flameTime * 100;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		moveVec.x += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		moveVec.x -= 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		moveVec.z += 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		moveVec.z -= 1.0f;
	}
	transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
}

void Player::UpdateAttack()
{
	//武器構え状態
	UpdateHide();
	if (cameraMode == THIRD_PERSON)
	{
		camera->ChangeViewMode(false);
		cameraMode = FIRST_PERSON;
	}
}

void Player::UpdateDead()
{
	// 死亡処理
}

void Player::ChangeState(State newState)
{
	state_ = newState;
}

void Player::MouseInput()
{
	moveX = (int)Input::GetMouseMoveX();
	moveY = (int)Input::GetMouseMoveY();
}

bool Player::IsCheckMoveInput()
{
	if (CheckHitKey(KEY_INPUT_W) ||
		CheckHitKey(KEY_INPUT_A) ||
		CheckHitKey(KEY_INPUT_S) ||
		CheckHitKey(KEY_INPUT_D))
	{
		return true;
	}
	return false;
}

void Player::CameraControl()
{
	transform.rotation.y += moveX * 0.5f * DegToRad;

	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position;
	camera->SetTargetPosition(lookPos);
}
