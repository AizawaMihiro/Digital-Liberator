#pragma once
#include <fbxsdk.h>
#include <string>

/// FBXファイルを読み込むクラス
/// RayCastの機能はObject3Dクラスに実装する予定
class Fbx
{
public:
	Fbx();
	Fbx(std::string filename);
	~Fbx();
	void Load(std::string filename);
};

