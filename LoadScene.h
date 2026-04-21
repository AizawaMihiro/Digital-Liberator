#pragma once
#include "Library/SceneBase.h"
#include "Library/Time.h"

/// <summary>
/// PlaySceneからPlaySceneに遷移させるときに表示するシーン
/// </summary>
class LoadScene : public SceneBase
{
public:
    LoadScene();
    ~LoadScene();

    void Update() override;
    void Draw() override;

private:
	float timer_;
};