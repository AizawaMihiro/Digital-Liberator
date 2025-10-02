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
