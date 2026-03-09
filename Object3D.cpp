#include "Object3D.h"

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

void Object3D::RayCast(Object3D object, RayCastData& data)
{
	FLOAT3 target = { data.origin.x + data.dir.x, data.origin.y + data.dir.y, data.origin.z + data.dir.z };
	MATRIX InvMat = MInverse(transform.GetLocalMatrix());
	VECTOR3 localOrigin = VTransform(data.origin, InvMat);
	VECTOR3 localTarget = VTransform(target, InvMat);
	VECTOR3 localDir = VSub(localTarget, localOrigin);

	// RayCastDataの更新
	data.origin = localOrigin;
	data.dir = localDir;
	data.hit = false;

	//objectの三角形とレイの当たり判定を行う
	//三角形の頂点は仮置き

	HITRESULT_LINE result =HitCheck_Line_Triangle(
		data.origin, data.origin + data.dir, 
		object.transform.position, object.transform.position + VECTOR3(0, 1, 0), object.transform.position + VECTOR3(1, 0, 0));

	data.hit = result.HitFlag;
	if (result.HitFlag)
	{
		data.pos = result.Position;
	}
}

