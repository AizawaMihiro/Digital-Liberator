#include "Enemy.h"
#include "ImGui/imgui.h"
#include "Map.h"
#include "Player.h"
#include "Time.h"

namespace {
	VECTOR3 defPos = { (-30.0f),(0.0f),(50.0f) };
	VECTOR3 defScale = { (0.5f),(0.25f),(0.5f) };
	VECTOR3 viewDefRot = { (0.0f),(180.0f * DegToRad),(0.0f) };
	VECTOR3 viewDefScale = { (0.25f),(0.25f),(0.25f) };
}

Enemy::Enemy()
	:state_(PATROL)
{
	hModel = MV1LoadModel("Assets/model/enemy01.mv1");//当たり判定用のモデル
	assert(hModel != -1);
	transform.position = defPos;
	transform.rotation = VZero;
	transform.scale = defScale;
	currentPatrolIndex = 0;
	flameTime_ = 0.0f;
	stunTimer_ = 0.0f;
	returndFlag_ = false;

	hViewModel_ = MV1LoadModel("Assets/model/character-g.mv1");
	assert(hViewModel_ != -1);
	viewModelTransform.position = VZero;
	viewModelTransform.rotation = viewDefRot;
	viewModelTransform.scale = viewDefScale;
	animTimer_ = 0.0f;
	animFrame_ = MV1AttachAnim(hViewModel_, Anim::WALK);

	hCheckSound_ = LoadSoundMem("Assets/sound/se/Check.mp3");

	MV1SetupCollInfo(hModel, -1);
}

Enemy::~Enemy()
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
	DeleteSoundMem(hCheckSound_);
}

void Enemy::Update()
{
	flameTime_ = Time::DeltaTime();
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player)
	{
		//プレイヤーとの距離を測り、一定距離以内かつEnemyの向きにいる場合はCHASE状態にする
		bool inChaseRange = false;
		VECTOR3 playerPos = player->GetTransform().position;
		int distX = abs(playerPos.x - this->transform.position.x);
		int distZ = abs(playerPos.z - this->transform.position.z);
		if (distX < ENEMY::CHASE_RANGE && distZ < ENEMY::CHASE_RANGE)
		{
			inChaseRange = true;
		}
		//Enemyの向きにいるかどうかを判定する
		bool inFront = false;
		MV1_COLL_RESULT_POLY result = this->RayCast(player, ENEMY::CHASE_RANGE);
		if (result.HitFlag == 1)
		{
			inFront = true;
		}

		//CHASE状態への遷移条件は、元々PATROLかRETURN状態でプレイヤーが一定距離以内かつEnemyの向きにいる場合
		//RETURN状態への遷移条件は、CHASE状態でプレイヤーが一定距離以上かつEnemyの向きにいない場合、またはRETURN状態でpatrolPointsに戻っていない場合
		//PATROL状態への遷移条件は、プレイヤーが一定距離以上かつEnemyの向きにいない場合、またはRETURN状態でpatrolPointsに戻った場合
		if (inChaseRange && inFront && (state_ == State::PATROL || state_ == State::RETURN || state_ == State::CHASE))
		{
			ChangeState(State::CHASE);
		}
		else if ((state_ == State::CHASE || state_ == State::RETURN) && !returndFlag_)
		{
			ChangeState(State::RETURN);
		}
		else if (state_ == State::PATROL || returndFlag_)
		{
			ChangeState(State::PATROL);
		}
	}


	switch (state_)
	{
	case State::PATROL:
		UpdatePatrol();
		break;
	case State::CHASE:
		UpdateChase();
		break;
	case State::RETURN:
		UpdateReturn();
		break;
	case State::STUN:
		UpdateStun();
		break;
	default:
		break;
	}

	viewModelTransform.position = transform.position;
	viewModelTransform.rotation = transform.rotation + VECTOR3(0.0f, 180.0f * DegToRad, 0.0f);

	UpdateViewModel();

	//DebugImgui();
}

void Enemy::Draw()
{
	if (hModel != -1)
	{
		Object3D::Draw();
	}
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
	MV1DrawModel(hViewModel_);
	//向き表示用の座標軸描画
	unsigned int red = GetColor(255, 0, 0);
	float hight = 40.0f;
	DrawLine3D(transform.position + VECTOR3(0, hight, -50) * MGetRotY(transform.rotation.y),
		transform.position + VECTOR3(0, hight, 50) * MGetRotY(transform.rotation.y), red);
	DrawCone3D(transform.position + VECTOR3(0, hight, 50) * MGetRotY(transform.rotation.y),
		transform.position + VECTOR3(0, hight, 40) * MGetRotY(transform.rotation.y), 5, 10, red, red, 1);
}

void Enemy::SetPosition(VECTOR3 pos)
{
	transform.position = pos;
	homePosition_ = VECTOR3(transform.position.x, transform.position.y, transform.position.z + BLOCK::HIT_SIZE * 2);
	//homePositionを中心に、周囲8マスの座標をpatrolPointsに追加する
	//左下から逆時計回りに追加する
	int offset = BLOCK::HIT_SIZE * 2;
	patrolPoints_.push_back(homePosition_ + VECTOR3(-offset, 0, offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(0, 0, offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(offset, 0, offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(offset, 0, 0));
	patrolPoints_.push_back(homePosition_ + VECTOR3(offset, 0, -offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(0, 0, -offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(-offset, 0, -offset));
	patrolPoints_.push_back(homePosition_ + VECTOR3(-offset, 0, 0));

	currentPatrolIndex = 0;
	transform.position = patrolPoints_[currentPatrolIndex];
}

/// <summary>
/// EnemyとPlayerがぶつかったかを返す
/// </summary>
/// <param name="pPos">PlayerPosition</param>
/// <param name="pSca">PlayerScale</param>
bool Enemy::CheckHitPlayer(VECTOR3 pPos, VECTOR3 pSca)
{
	float distX = abs(pPos.x - this->transform.position.x);
	float distZ = abs(pPos.z - this->transform.position.z);
	float limitX = pSca.x + this->transform.scale.x;
	float limitZ = pSca.z + this->transform.scale.z;
	//モデルのサイズの都合で当たり判定が乱れ中
	if (distX < limitX && distZ < limitZ)
	{
		return true;
	}
	return false;
}

void Enemy::SetStateStun()
{
	//スタン状態になったときの処理
	//スタン時間をリセットするなど
	ChangeState(State::STUN);
	stunTimer_ = ENEMY::STUN_TIME;
}

void Enemy::UpdateCollision()
{
	//RayCastで当たり判定を行うためのコリジョン情報を更新する
	MV1RefreshCollInfo(hModel);
}

void Enemy::UpdatePatrol()
{
	//Mapから情報をもらい、自分の位置と比較してPathを回る
	//patrolPointsを順番に回るようにする
	Map* map = ObjectManager::FindGameObject<Map>();
	if (map != nullptr)
	{
		std::vector<std::vector<int>> MapData = map->GetMapData();
		//自分の位置をマップ上の座標に変換して保存する
		int myMapX = static_cast<int>((this->GetTransform().position.x + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		int myMapY = static_cast<int>((this->GetTransform().position.z + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		//マップ上の座標とpatrolPointsを比較して、次のpatrolPointを決める
		int targetMapX = static_cast<int>((patrolPoints_[currentPatrolIndex].x + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		int targetMapY = static_cast<int>((patrolPoints_[currentPatrolIndex].z + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		if (myMapX == targetMapX && myMapY == targetMapY)
		{
			currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints_.size();
		}
		else
		{
			//次のpatrolPointに向かう
			VECTOR3 direction = patrolPoints_[currentPatrolIndex] - this->GetTransform().position;
			float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
			if (length > 0.1f)
			{
				direction /= length; //正規化
				float flameMoveDist = ENEMY::MOVE_SPEED * flameTime_ * 100;
				VECTOR3 moveVec = { 0.0f,0.0f,1.0f };
				transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
				transform.rotation.y = atan2(direction.x, direction.z); //向きを変える
			}
		}
	}

	if (returndFlag_)
	{
		returndFlag_ = false;
	}
}

void Enemy::UpdateChase()
{
	//プレイヤーの位置を取得して、プレイヤーに向かって移動する
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player)
	{
		VECTOR3 playerPos = player->GetTransform().position;
		VECTOR3 direction = playerPos - this->GetTransform().position;
		float length = sqrt(direction.x * direction.x + direction.z * direction.z);
		if (length > 0.1f)
		{
			direction /= length; //正規化
			float flameMoveDist = ENEMY::CHASE_SPEED * flameTime_ * 100;
			VECTOR3 moveVec = { 0.0f,0.0f,1.0f };
			transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
			transform.rotation.y = atan2(direction.x, direction.z); //向きを変える
		}
	}
}

void Enemy::UpdateReturn()
{
	//本来向かおうとしていたpatrolPointに向かう
	VECTOR3 direction = patrolPoints_[currentPatrolIndex] - this->GetTransform().position;
	float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (length > 1.0f)//patrolPointに戻るときは少し距離の閾値を大きくする
	{
		direction /= length; //正規化
		float flameMoveDist = ENEMY::MOVE_SPEED * flameTime_ * 100;
		VECTOR3 moveVec = { 0.0f,0.0f,1.0f };
		transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
		transform.rotation.y = atan2(direction.x, direction.z); //向きを変える
	}
	else
	{
		//patrolPointに戻ったときの処理
		returndFlag_ = true;
	}
}

void Enemy::UpdateStun()
{
	//プレイヤーに攻撃されたときに停止する
	//時間経過で回復
	stunTimer_ -= flameTime_;
	if (stunTimer_ <= 0)
	{
		ChangeState(State::PATROL);
		animFrame_ = MV1AttachAnim(hViewModel_, Anim::WALK);
	}
}

void Enemy::UpdateViewModel()
{
	float totalTime = MV1GetAttachAnimTotalTime(hViewModel_, animFrame_);
	float animSpeed = flameTime_ * 60.0f;//アニメーションの再生速度を調整するための値
	animTimer_ = fmod(animTimer_ + animSpeed, totalTime);
	MV1SetAttachAnimTime(hViewModel_, animFrame_, animTimer_);
}

void Enemy::ChangeState(State newState)
{
	if (newState!=state_)
	{
		if (newState == State::CHASE)
		{
			PlaySoundMem(hCheckSound_, DX_PLAYTYPE_BACK);
		}
		if (newState == State::STUN)
		{
			animFrame_ = MV1AttachAnim(hViewModel_, Anim::STOP);
		}
		state_ = newState;
	}
}

void Enemy::DebugImGui()
{
	ImGui::Begin("Enemy");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::End();
}

