#include "Goalpost.h"
#include <assert.h>
#include "Player.h"
#include "ImGui/imgui.h"

Goalpost::Goalpost()
{
	hModel = MV1LoadModel("Assets/model/Ball.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	VECTOR3 defPos = { (0.0f),(0.0f),(0.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { (GOALPOST::size),(GOALPOST::size),(GOALPOST::size) };
	transform.scale = defScale;

	TouchGoal = false;
}

Goalpost::~Goalpost()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Goalpost::Update()
{
	//アニメーションなど

	//接触
	Player* player = ObjectManager::FindGameObject<Player>();
	if (player)
	{
		VECTOR3 playerPos = player->GetTransform().position;
		VECTOR3 playerScale = player->GetTransform().scale;
		if (CheckHitPlayer(playerPos, playerScale))
		{
			TouchGoal = true;
		}
	}

	ImGui::Begin("Goal");
	ImGui::InputFloat("PositionX", &transform.position.x);
	ImGui::InputFloat("PositionY", &transform.position.y);
	ImGui::InputFloat("PositionZ", &transform.position.z);
	ImGui::End();
}

void Goalpost::Draw()
{
	Object3D::Draw();
	MV1DrawModel(hModel);
}

bool Goalpost::CheckHitPlayer(VECTOR3 pPos, VECTOR3 pSca)
{
	float distX = abs(pPos.x - this->GetTransform().position.x);
	float distZ = abs(pPos.z - this->GetTransform().position.z);
	float limitX = pSca.x + GOALPOST::size;
	float limitZ = pSca.z + GOALPOST::size;
	if (distX < limitX && distZ < limitZ)
	{
		return true;
	}
	return false;
}
