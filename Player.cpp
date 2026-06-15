#include "Player.h"
#include "Camera.h"
#include "ImGui/imgui.h"
#include "Input.h"
#include "Enemy.h"
#include "Time.h"
#include "AmmoCounter.h"
#include "Object2D.h"

namespace
{
	VECTOR3 defScale = { (10.0f),(10.0f),(10.0f) };
	VECTOR3 viewDefRot = { (0.0f),(180.0f * DegToRad),(0.0f) };
	VECTOR3 viewDefScale = { (0.2f),(0.2f),(0.2f) };
}

Player::Player()
	:state_(IDLE), cameraMode_(THIRD_PERSON), gunAmmo_(0), leftClicked_(false)
{
	hModel = MV1LoadModel("Assets/model/cube.mv1");//当たり判定用のモデル
	assert(hModel != -1);
	transform.position = VZero;
	transform.rotation = VZero;
	transform.scale = defScale;

	viewModelTransform.position = VZero;
	viewModelTransform.rotation = viewDefRot;
	viewModelTransform.scale = viewDefScale;

	hIdleAnim_ = MV1LoadModel("Assets/anime/Idle.mv1");
	hMoveAnim_ = MV1LoadModel("Assets/anime/Run.mv1");
	hViewModel_ = hIdleAnim_;
	animTimer_ = 0.0f;
	animFrame_ = MV1AttachAnim(hViewModel_, 0);

	MV1SetupCollInfo(hModel, -1);
	camera_ = FindGameObject<Camera>();
	flameTime_ = Time::DeltaTime();

	hWalkSound_ = LoadSoundMem("Assets/sound/se/walk.mp3");
	hDashSound_ = LoadSoundMem("Assets/sound/se/run.mp3");
	hAttackReadySound_ = LoadSoundMem("Assets/sound/se/gunready.mp3");
	hAttackSound_ = LoadSoundMem("Assets/sound/se/shot.mp3");
	hReloadSound_ = LoadSoundMem("Assets/sound/se/reload.mp3");
	hAmmoEmptySound_ = LoadSoundMem("Assets/sound/se/noammo.mp3");

	uiCrosshair_ = new Object2D();
	uiCrosshair_->SetGraph("Assets/image/Crosshair.png");
	Transform crosshairTransform;
	crosshairTransform.position = VECTOR3(600.0f, 300.0f, 0.0f);
	crosshairTransform.rotation = VZero;
	crosshairTransform.scale = VECTOR3(1.0f, 1.0f, 1.0f);
	uiCrosshair_->SetTransform(crosshairTransform);
	uiCrosshair_->SetDrawFlag(false);
	uiCrosshair_->SetTransFlag(1);

	gunAmmo_ = AmmoCounter::GetCurrentAmmo();
	AmmoCounter::SetDisplayAmmo();
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
	DeleteSoundMem(hAttackReadySound_);
	DeleteSoundMem(hAttackSound_);
	DeleteSoundMem(hReloadSound_);
	DeleteSoundMem(hAmmoEmptySound_);
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

	if (cameraMode_ == FIRST_PERSON && state_ != ATTACK)
	{
		camera_->ChangeViewMode(true);
		cameraMode_ = THIRD_PERSON;
	}

	CameraControl();
	UpdateViewModel();

	MV1RefreshCollInfo(hModel);

	AmmoCounter::DrawDisplay();

	//DebugImGui();
}

void Player::Draw()
{
	// プレイヤーモデルの描画準備
	// この準備をしておかないと、RayCastの結果がおかしくなる
	if (hModel != -1)
	{
		Object3D::Draw();
	}
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

	// カメラの視点に応じて描画するものを切り替える
	if (camera_->IsThirdPerson())
	{
		MV1DrawModel(hViewModel_);
		uiCrosshair_->SetDrawFlag(false);
	}
	else
	{
		// クロスヘアの描画
		uiCrosshair_->SetDrawFlag(true);
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

	//歩いているときのSEの再生
	if (IsCheckMoveInput()&& !CheckSoundMem(hWalkSound_))
	{
		PlaySoundMem(hWalkSound_, DX_PLAYTYPE_LOOP);
	}
	else if (!IsCheckMoveInput() && CheckSoundMem(hWalkSound_))
	{
		StopSoundMem(hWalkSound_);
	}
	
	if (cameraMode_ == THIRD_PERSON)
	{
		camera_->ChangeViewMode(false);
		cameraMode_ = FIRST_PERSON;
	}

	//照準操作
	//マウスの移動量を取得して、プレイヤーの向きに反映させる
	//上下の照準操作はカメラの回転で行うので、ここでは左右の照準操作のみ行う
	transform.rotation.y += moveX * TARGETING_ROT_SPEED * DegToRad;

	//攻撃処理
	if (gunAmmo_ > 0)
	{
		if (GetMouseInput() && MOUSE_INPUT_LEFT)
		{
			//左クリックされた瞬間に攻撃処理を行う
			if (!leftClicked_)
			{
				leftClicked_ = true;
				PlaySoundMem(hAttackSound_, DX_PLAYTYPE_BACK);
				std::list<Enemy*> enemies = ObjectManager::FindGameObjects<Enemy>();
				for (Enemy* enemy : enemies)
				{
					//Playerの向きにいるかどうかを判定する
					VECTOR3 enemyPos = enemy->GetTransform().position;
					VECTOR3 forward = { 0.0f,0.0f,1.0f };
					forward = forward * MGetRotY(transform.rotation.y);
					VECTOR3 toPlayer = enemyPos - this->transform.position;
					float dot = forward.x * toPlayer.x + forward.y * toPlayer.y + forward.z * toPlayer.z;
					if (dot > 0)
					{
						//Playerの向きにいると判定された敵に対して、RayCastを行う
						//RayCastの結果、攻撃が当たったと判定された敵はスタン状態にする
						enemy->UpdateCollision();//RayCastの前にコリジョン情報を更新する必要がある
						MV1_COLL_RESULT_POLY result = RayCast(enemy, ATTACK_RANGE);
						if (result.HitFlag)
						{
							enemy->SetStateStun();
						}
					}
				}
			}
			//左クリックが押され続けている間は攻撃処理を行わない
		}
		else
		{
			if (leftClicked_)
			{
				PlaySoundMem(hReloadSound_, DX_PLAYTYPE_BACK);
				gunAmmo_--;
				AmmoCounter::CountDown();
				leftClicked_ = false;
			}
		}
	}
	else
	{
		if (GetMouseInput() && MOUSE_INPUT_LEFT) {
			if (!leftClicked_)
			{
				leftClicked_ = true;
				//弾切れの音を鳴らす
				PlaySoundMem(hAmmoEmptySound_, DX_PLAYTYPE_BACK);
			}
	
		}
		else
		{
			leftClicked_ = false;
		}
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
	float animSpeed = flameTime_ * 100.0f;
	animTimer_ = fmod(animTimer_ + animSpeed, totalTime);
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
	VECTOR3 camRot = camera_->GetCameraRot();
	camRot.y = transform.rotation.y;
	camera_->SetCameraRotation(camRot);

	// カメラの注視点をプレイヤーの前方に設定
	VECTOR3 lookPos = transform.position;
	camera_->SetTargetPosition(lookPos);
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
