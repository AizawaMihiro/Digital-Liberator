#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"
#include "Time.h"
#include "Object2D.h"

namespace
{
	VECTOR3 defScale = { (10.0f),(10.0f),(10.0f) };
	VECTOR3 viewDefRot = { (0.0f),(180.0f * DegToRad),(0.0f) };
	VECTOR3 viewDefScale = { (0.2f),(0.2f),(0.2f) };
}

Player::Player()
	:state_(IDLE), cameraMode(THIRD_PERSON)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	transform.position = VZero;
	transform.rotation = VZero;
	transform.scale = defScale;

	hViewModel_ = MV1LoadModel("Assets/model/3Dchara man.mv1");
	assert(hViewModel_ != -1);
	viewModelTransform.position = VZero;
	viewModelTransform.rotation = viewDefRot;
	viewModelTransform.scale = viewDefScale;

	hIdleAnim_ = MV1LoadModel("Assets/anime/Idle.mv1");
	hMoveAnim_ = MV1LoadModel("Assets/anime/Run.mv1");
	hViewModel_ = hIdleAnim_;
	animFrame_ = MV1AttachAnim(hViewModel_, 0);

	MV1SetupCollInfo(hModel, -1);
	camera = FindGameObject<Camera>();
	flameTime_ = Time::DeltaTime();

	hWalkSound_ = LoadSoundMem("Assets/sound/se/walk.mp3");
	hDashSound_ = LoadSoundMem("Assets/sound/se/run.mp3");

	uiCrosshair = new Object2D();
	uiCrosshair->SetGraph("Assets/image/Crosshair.png");
	Transform crosshairTransform;
	crosshairTransform.position = VECTOR3(600.0f, 300.0f, 0.0f);
	crosshairTransform.rotation = VZero;
	crosshairTransform.scale = VECTOR3(1.0f, 1.0f, 1.0f);
	uiCrosshair->SetTransform(crosshairTransform);
	uiCrosshair->SetDrawFlag(false);
}

Player::~Player()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
	if (hViewModel_ != -1)
	{
		MV1DeleteModel(hViewModel_);
		hViewModel_ = -1;
	}
	MV1DeleteModel(hIdleAnim_);
	MV1DeleteModel(hMoveAnim_);
	DeleteSoundMem(hWalkSound_);
	DeleteSoundMem(hDashSound_);
}

void Player::Update()
{
	flameTime_ = Time::DeltaTime();
	MouseInput();

	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		ChangeState(ATTACK);
	}
	else if (IsCheckMoveInput())
	{
		ChangeState(MOVE);
	}
	else
	{
		ChangeState(IDLE);
	}

	switch (state_)
	{
	case MOVE:
		UpdateMove();
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
	UpdateViewModel();

	MV1RefreshCollInfo(hModel);

	//DebugImGui();
}

void Player::Draw()
{
	// ビューモデルの描画準備
	if (hViewModel_ != -1)
	{
		const MATRIX& local = viewModelTransform.MakeLocalMatrix();
		if (parent != nullptr) {
			const MATRIX& parentLocal = parent->GetTransform().GetLocalMatrix();
			MATRIX world = local * parentLocal;
			MV1SetMatrix(hViewModel_, world);
		}
		else {
			MV1SetMatrix(hViewModel_, local);
		}
	}
	// プレイヤーモデルの描画準備
	// この準備をしておかないと、RayCastの結果がおかしくなる
	if (hModel != -1)
	{
		Object3D::Draw();
	}

	if (camera->IsThirdPerson())
	{
		MV1DrawModel(hViewModel_);
		//MV1DrawModel(hModel);
		uiCrosshair->SetDrawFlag(false);
	}
	else
	{
		// クロスヘアの描画
		uiCrosshair->SetDrawFlag(true);
	}

}

void Player::UpdateMove()
{
	float moveTime = flameTime_ * 100.0f;
	float flameMoveDist = MOVE_SPEED * moveTime;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform.rotation.y += ROT_SPEED * DegToRad * moveTime;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		transform.rotation.y -= ROT_SPEED * DegToRad * moveTime;
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

void Player::UpdateAttack()
{
	//武器構え状態
	//やや遅くなる
	float moveTime = flameTime_ * 100.0f;
	float flameMoveDist = HIDE_SPEED * moveTime;
	VECTOR3 moveVec = { 0.0f,0.0f,0.0f };
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform.rotation.y += ROT_SPEED * DegToRad * moveTime;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		transform.rotation.y -= ROT_SPEED * DegToRad * moveTime;
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
	
	if (cameraMode == THIRD_PERSON)
	{
		camera->ChangeViewMode(false);
		cameraMode = FIRST_PERSON;
	}

	//攻撃処理
	if (GetMouseInput() && MOUSE_INPUT_LEFT)
	{

	}
}

void Player::UpdateDead()
{
	// 死亡処理
}

void Player::ChangeState(State newState)
{
	//アニメーションの切り替え
	//seの切り替えもここで行う
	if (newState != state_)
	{
		if (newState == State::MOVE && hViewModel_ != hMoveAnim_)
		{
			MV1DetachAnim(hViewModel_, 0);
			hViewModel_ = hMoveAnim_;
			viewModelTransform.scale = viewDefScale;
			animTimer_ = 0.0f;
			animFrame_ = MV1AttachAnim(hViewModel_, 0);
			if (CheckSoundMem(hWalkSound_))
			{
				StopSoundMem(hWalkSound_);
			}
			PlaySoundMem(hDashSound_, DX_PLAYTYPE_LOOP);
		}
		else if (newState == State::ATTACK)
		{
			if (CheckSoundMem(hDashSound_))
			{
				StopSoundMem(hDashSound_);
			}
			PlaySoundMem(hWalkSound_, DX_PLAYTYPE_LOOP);
		}
		else if (newState == State::IDLE && hViewModel_ != hIdleAnim_)
		{
			MV1DetachAnim(hViewModel_, 0);
			hViewModel_ = hIdleAnim_;
			viewModelTransform.scale = viewDefScale;
			animTimer_ = 0.0f;
			animFrame_ = MV1AttachAnim(hViewModel_, 0);
			if (CheckSoundMem(hDashSound_))
			{
				StopSoundMem(hDashSound_);
			}
			if (CheckSoundMem(hWalkSound_))
			{
				StopSoundMem(hWalkSound_);
			}
		}
		state_ = newState;
	}
}

void Player::UpdateViewModel()
{
	float totalTime = MV1GetAttachAnimTotalTime(hViewModel_, animFrame_);
	animTimer_ = fmod(animTimer_ + flameTime_ * 100.0f, totalTime);
	MV1SetAttachAnimTime(hViewModel_, animFrame_, animTimer_);
}

void Player::MouseInput()
{
	// マウスの移動量を取得
	// 照準操作に使用予定
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
	// カメラの向きをプレイヤーの向きに合わせる
	VECTOR3 camRot = camera->GetCameraRot();
	camRot.y = transform.rotation.y;
	camera->SetCameraRotation(camRot);

	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position;
	camera->SetTargetPosition(lookPos);
}

void Player::DebugImGui()
{
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
