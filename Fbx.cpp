#include "Fbx.h"

Fbx::Fbx()
{
}

Fbx::Fbx(std::string filename)
{
	Load(filename);
}

Fbx::~Fbx()
{
}

void Fbx::Load(std::string filename)
{
	// FBXマネージャの作成
	FbxManager* manager = FbxManager::Create();
	// FBXインポータの作成
	FbxImporter* importer = FbxImporter::Create(manager, "");
	// FBXファイルの読み込み
	if (!importer->Initialize(filename.c_str(), -1, manager->GetIOSettings())) {
		printf("FBXファイルの読み込みに失敗: %s\n", importer->GetStatus().GetErrorString());
		return;
	}
	// シーンの作成
	FbxScene* scene = FbxScene::Create(manager, "My Scene");
	// FBXファイルをシーンにインポート
	importer->Import(scene);
	// インポータのクリーンアップ
	importer->Destroy();
	// ここでシーンからメッシュやアニメーションデータを抽出して処理することができます
}
