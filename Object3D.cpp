#include "Object3D.h"

Object3D::Object3D()
	: hModel(-1), parent(nullptr)
{
	GameObject::SetDrawOrder(100);//3Dオブジェクトは2Dオブジェクトより先に描画されるようにする
}

Object3D::~Object3D()
{
	if (hModel != -1) {
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Object3D::Draw()
{
	if (hModel != -1)
	{
		const MATRIX& local = transform.MakeLocalMatrix();
		if (parent != nullptr) {
			const MATRIX& parentLocal = parent->GetTransform().GetLocalMatrix();
			MATRIX world = local * parentLocal;
			MV1SetMatrix(hModel, world);
		}
		else {
			MV1SetMatrix(hModel, local);
		}
	}
}

MV1_COLL_RESULT_POLY Object3D::RayCast(Object3D* target, float distance)
{
	VECTOR3 forward = { 0.0f,0.0f,1.0f };
	VECTOR3 startPos = this->transform.position + VECTOR3(0.0f, 20.0f, 0.0f) * MGetRotY(transform.rotation.y);
	VECTOR3 endPos = startPos + forward * distance * MGetRotY(transform.rotation.y);
	DrawLine3D(startPos, endPos, GetColor(255, 255, 0));
	return MV1CollCheck_Line(target->hModel,-1, startPos, endPos);
}
