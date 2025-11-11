#pragma once
#include "Object3D.h"
class Block :
    public Object3D
{
public:
	Block();
	~Block();
	void Draw() override;
private:
};

