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
private:
    int hImage;
    Transform transform;

};

