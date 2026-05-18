#pragma once
#include "Library/GameObject.h"
#include "Transform.h"

class Object2D :
    public GameObject
{
public:
    Object2D();
    ~Object2D();
    void Draw()override;
    const Transform& GetTransform() { return transform; }
    const void SetTransform(const Transform& _transform) { transform = _transform; }
    void SetGraph(std::string filename);
	void SetDrawFlag(bool flag) { drawFlag = flag; }
	void SetTransFlag(int flag) { transFlag = flag; }//透過フラグ　1なら透過する
private:
    int hImage;
	bool drawFlag;
	int transFlag;
    Transform transform;
};

