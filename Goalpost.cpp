#include "Goalpost.h"
#include <assert.h>
#include "ImGui/imgui.h"

Goalpost::Goalpost()
{
	hModel = MV1LoadModel("Assets/model/Ball.mv1");//まだモデルがないので仮
	assert(hModel != -1);
	VECTOR3 defPos = { (0.0f),(0.0f),(0.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { (size),(size),(size) };
	transform.scale = defScale;
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
