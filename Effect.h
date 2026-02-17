#pragma once
#include "Object3D.h"

class Effect :
    public Object3D
{
    Effect();
    Effect(int handle, int time);
    ~Effect();
    void Update() override;
    void Draw() override;
private:
    int hEffect;
    int LimitTime;
    int nowTime;
};

