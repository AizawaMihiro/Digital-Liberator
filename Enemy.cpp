#include "Enemy.h"
#include "ImGui/imgui.h"

Enemy::Enemy()
	:state(PATROL)
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
	homePosition = VECTOR3(transform.position.x, transform.position.y, transform.position.z + BLOCK::SIZE * 2);
	for (int z = -1; z <= 1; z++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (z == 0 && x == 0) {
				continue;
			}
			patrolPoints.push_back(VECTOR3(homePosition.x + x * BLOCK::SIZE * 2, homePosition.y, homePosition.z + z * BLOCK::SIZE * 2));
		}
	}
	currentPatrolIndex = 0;
	transform.position = patrolPoints[currentPatrolIndex];
	transform.rotation.y = 90.0f * DegToRad;
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
	if (distX < limitX && distZ < limitZ)
	{
		return true;
	}
	return false;
}

void Enemy::UpdatePatrol()
{
	//Mapから情報をもらい、自分の位置と比較してPathを回る
	//優先順位はL,R,U,D
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

