#include "Enemy.h"
#include "ImGui/imgui.h"
#include "Map.h"
#include "Time.h"

Enemy::Enemy()
	:state(PATROL)
{
	hModel = MV1LoadModel("Assets/model/enemy01.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	VECTOR3 defPos = { (-30.0f),(0.0f),(50.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { (0.5f),(0.25f),(0.5f) };
	transform.scale = defScale;
	//transform.rotation.y = 90.0f * DegToRad;
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
	switch (state)
	{
	case PATROL:
		UpdatePatrol();
		break;
	case CHASE:
		UpdateChase();
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
	homePosition = VECTOR3(transform.position.x, transform.position.y, transform.position.z + BLOCK::HIT_SIZE * 2);
	//homePositionを中心に、周囲8マスの座標をpatrolPointsに追加する
	//左下から逆時計回りに追加する
	int offset = BLOCK::HIT_SIZE * 2;
	patrolPoints.push_back(homePosition + VECTOR3(-offset, 0, offset));
	patrolPoints.push_back(homePosition + VECTOR3(0, 0, offset));
	patrolPoints.push_back(homePosition + VECTOR3(offset, 0, offset));
	patrolPoints.push_back(homePosition + VECTOR3(offset, 0, 0));
	patrolPoints.push_back(homePosition + VECTOR3(offset, 0, -offset));
	patrolPoints.push_back(homePosition + VECTOR3(0, 0, -offset));
	patrolPoints.push_back(homePosition + VECTOR3(-offset, 0, -offset));
	patrolPoints.push_back(homePosition + VECTOR3(-offset, 0, 0));

	currentPatrolIndex = 0;
	transform.position = patrolPoints[currentPatrolIndex];
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
		int targetMapX = static_cast<int>((patrolPoints[currentPatrolIndex].x + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		int targetMapY = static_cast<int>((patrolPoints[currentPatrolIndex].z + BLOCK::HIT_SIZE) / (BLOCK::HIT_SIZE * 2));
		if (myMapX == targetMapX && myMapY == targetMapY)
		{
			currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
		}
		else
		{
			//次のpatrolPointに向かう
			VECTOR3 direction = patrolPoints[currentPatrolIndex] - this->GetTransform().position;
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
	//プレイヤーが近くにいる場合、位置（マス）を受け取り向かう
}

void Enemy::UpdateStun()
{
	//プレイヤーに攻撃されたときに停止する
	//時間経過で回復
}

void Enemy::ChangeState(State newState)
{
	state = newState;
}

