#include "Enemy.h"
#include "ImGui/imgui.h"
#include "Map.h"
#include "Player.h"
#include "Time.h"

Enemy::Enemy()
	:state_(PATROL)
{
	hModel = MV1LoadModel("Assets/model/enemy01.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	VECTOR3 defPos = { (-30.0f),(0.0f),(50.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { (0.5f),(0.25f),(0.5f) };
	transform.scale = defScale;
}

Enemy::~Enemy()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Enemy::Update()
{
	flameTime = Time::DeltaTime();
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
		VECTOR3 forward = { 0.0f,0.0f,1.0f };
		forward = forward * MGetRotY(transform.rotation.y);
		VECTOR3 toPlayer = playerPos - this->transform.position;
		float dot = forward.x * toPlayer.x + forward.y * toPlayer.y + forward.z * toPlayer.z;
		if (dot > 0)
		{
			inFront = true;
		}

		//CHASE状態への遷移条件は、元々PATROLかRETURN状態でプレイヤーが一定距離以内かつEnemyの向きにいる場合
		//PATROL状態への遷移条件は、プレイヤーが一定距離以上かつEnemyの向きにいない場合、またはRETURN状態でpatrolPointsに戻った場合
		//RETURN状態への遷移条件は、CHASE状態でプレイヤーが一定距離以上かつEnemyの向きにいない場合、またはRETURN状態でpatrolPointsに戻っていない場合
		if (inChaseRange && inFront && (state_ == State::PATROL || state_ == State::RETURN))
		{
			ChangeState(CHASE);
		}
		else if (state_ == State::PATROL || returndFlag)
		{
			ChangeState(PATROL);
		}
		else if (state_ == State::CHASE || state_ == State::RETURN)
		{
			ChangeState(RETURN);
		}
	}


	switch (state_)
	{
	case PATROL:
		UpdatePatrol();
		break;
	case CHASE:
		UpdateChase();
		break;
	case RETURN:
		UpdateReturn();
		break;
	case STUN:
		UpdateStun();
		break;
	default:
		break;
	}

	//ImGui::Begin("Enemy");
	//ImGui::InputFloat("PositionX", &transform.position.x);
	//ImGui::InputFloat("PositionY", &transform.position.y);
	//ImGui::InputFloat("PositionZ", &transform.position.z);
	//ImGui::End();
}

void Enemy::Draw()
{
	Object3D::Draw();
	MV1DrawModel(hModel);
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
	ChangeState(STUN);
	stunTimer = ENEMY::STUN_TIME;
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
				float flameMoveDist = ENEMY::MOVE_SPEED * flameTime * 100;
				VECTOR3 moveVec = { 0.0f,0.0f,1.0f };
				transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
				transform.rotation.y = atan2(direction.x, direction.z); //向きを変える
			}
		}
	}
}

void Enemy::UpdateChase()
{
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player)
	{
		VECTOR3 playerPos = player->GetTransform().position;
		VECTOR3 direction = playerPos - this->GetTransform().position;
		float length = sqrt(direction.x * direction.x + direction.z * direction.z);
		if (length > 0.1f)
		{
			direction /= length; //正規化
			float flameMoveDist = ENEMY::CHASE_SPEED * flameTime * 100;
			VECTOR3 moveVec = { 0.0f,0.0f,1.0f };
			transform.position += moveVec.Normalize() * flameMoveDist * MGetRotY(transform.rotation.y);
			transform.rotation.y = atan2(direction.x, direction.z); //向きを変える
		}
	}
}

void Enemy::UpdateReturn()
{

}

void Enemy::UpdateStun()
{
	//プレイヤーに攻撃されたときに停止する
	//時間経過で回復
	stunTimer -= flameTime;
	if (stunTimer <= 0)
	{
		ChangeState(PATROL);
	}
}

void Enemy::ChangeState(State newState)
{
	state_ = newState;
}

