#include "Block.h"
#include <assert.h>

Block::Block()
	:Object3D()
{
	hModel = MV1LoadModel("Assets/model/Block.mv1");//‚Ü‚¾ƒ‚ƒfƒ‹‚ª‚È‚¢‚Ì‚Å‰¼
	assert(hModel != -1);
	VECTOR3 defPos = { (50.0f),(0.0f),(50.0f) };
	transform.position = defPos;
	VECTOR3 defScale = { BLOCK::SIZE,BLOCK::SIZE,BLOCK::SIZE };
	transform.scale = defScale;
}

Block::~Block()
{
	if (hModel != -1)
	{
		MV1DeleteModel(hModel);
		hModel = -1;
	}
}

void Block::Draw()
{
	Object3D::Draw();
	MV1DrawModel(hModel);
}
