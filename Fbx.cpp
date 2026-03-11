#include "Fbx.h"

Fbx::Fbx():
	ppIndexBuffer_(nullptr), pMaterial_(nullptr),
	pVertexBuffer_(nullptr), pConstantBuffer_(nullptr),
	pVertexData_(nullptr), ppIndexData_(nullptr)
{
}

Fbx::Fbx(std::string filename) : Fbx()
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

HRESULT Fbx::Init(FbxNode* pNode)
{//ノードからメッシュの情報を取得
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();			//頂点の数
	polygonCount_ = mesh->GetPolygonCount();				//ポリゴンの数
	polygonVertexCount_ = mesh->GetPolygonVertexCount();	//ポリゴン頂点インデックス数 


	InitVertex(mesh);		//頂点バッファ準備
	InitMaterial(pNode);	//マテリアル準備
	InitIndex(mesh);		//インデックスバッファ準備
	IntConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備

	return E_NOTIMPL;
}

void Fbx::RayCast(RayCastData* data)
{
	data->hit = FALSE;

	//マテリアル毎
	for (DWORD i = 0; i < materialCount_; i++)
	{
		//そのマテリアルのポリゴン毎
		for (DWORD j = 0; j < pMaterial_[i].polygonCount; j++)
		{
			//3頂点
			FLOAT3 ver[3];
			ver[0] = pVertexData_[ppIndexData_[i][j * 3 + 0]].position;
			ver[1] = pVertexData_[ppIndexData_[i][j * 3 + 1]].position;
			ver[2] = pVertexData_[ppIndexData_[i][j * 3 + 2]].position;

			HITRESULT_LINE result;
			result = HitCheck_Line_Triangle(data->origin, data->origin+data->dir, ver[0], ver[1], ver[2]);
			
			if (result.HitFlag)
			{
				data->hit = result.HitFlag;
				data->pos = result.Position;
			}
		}
	}
}

void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	pVertexData_ = new VERTEX[vertexCount_];

	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			int index = mesh->GetPolygonVertex(poly, vertex);

			/////////////////////////頂点の位置/////////////////////////////////////
			FbxVector4 pos = mesh->GetControlPointAt(index);
			pVertexData_[index].position = FLOAT3{ ((float)pos[0], (float)pos[1], (float)pos[2]) };

			/////////////////////////頂点の法線/////////////////////////////////////
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			pVertexData_[index].normal = FLOAT3{ ((float)Normal[0], (float)Normal[1], (float)Normal[2]) };

			///////////////////////////頂点のＵＶ/////////////////////////////////////
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			pVertexData_[index].uv = FLOAT3{ ((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f) };
		}
	}


	///////////////////////////頂点のＵＶ/////////////////////////////////////
	int m_dwNumUV = mesh->GetTextureUVCount();
	FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
	if (m_dwNumUV > 0 && pUV->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		for (int k = 0; k < m_dwNumUV; k++)
		{
			FbxVector2 uv = pUV->GetDirectArray().GetAt(k);
			pVertexData_[k].uv = FLOAT3{ ((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f) };
		}
	}


	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * mesh->GetControlPointsCount();
	bd_vertex.Usage = D3D11_USAGE_DYNAMIC;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = pVertexData_;
	//Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{

	// マテリアルバッファの生成
	materialCount_ = pNode->GetMaterialCount();
	pMaterial_ = new MATERIAL[materialCount_];

	for (DWORD i = 0; i < materialCount_; i++)
	{
		ZeroMemory(&pMaterial_[i], sizeof(pMaterial_[i]));

		// フォンシェーディングを想定したマテリアルバッファの抽出
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		// 環境光＆拡散反射光＆鏡面反射光の反射成分値を取得
		FbxDouble3  ambient = FbxDouble3(0, 0, 0);
		FbxDouble3  diffuse = FbxDouble3(0, 0, 0);
		FbxDouble3  specular = FbxDouble3(0, 0, 0);
		ambient = pPhong->Ambient;
		diffuse = pPhong->Diffuse;

		// 環境光＆拡散反射光＆鏡面反射光の反射成分値をマテリアルバッファにコピー
		pMaterial_[i].ambient = FLOAT4{ ((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f) };
		pMaterial_[i].diffuse = FLOAT4{ ((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f) };
		pMaterial_[i].specular = FLOAT4{ (0, 0, 0, 0) };
		pMaterial_[i].shininess = 0;


		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			specular = pPhong->Specular;
			pMaterial_[i].specular = FLOAT4{ ((float)specular[0], (float)specular[1], (float)specular[2], 1.0f) };
			pMaterial_[i].shininess = (float)pPhong->Shininess;
		}

		InitTexture(pMaterial, i);

	}
}

void Fbx::InitTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, const DWORD& i)
{
	// テクスチャー情報の取得
	FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//テクスチャの数
	int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

	if (fileTextureCount > 0)
	{
		FbxFileTexture* texture = lProperty.GetSrcObject<FbxFileTexture>(0);

		//ファイル名+拡張だけにする
		char name[_MAX_FNAME];	//ファイル名
		char ext[_MAX_EXT];		//拡張子
		_splitpath_s(texture->GetRelativeFileName(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
		wsprintf(name, "%s%s", name, ext);

	}
}

void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	// マテリアルの数だけインデックスバッファーを作成
	ppIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	ppIndexData_ = new DWORD * [materialCount_];

	int count = 0;

	// マテリアルから「ポリゴン平面」の情報を抽出する
	for (DWORD i = 0; i < materialCount_; i++)
	{
		count = 0;
		DWORD* pIndex = new DWORD[polygonCount_ * 3];
		ZeroMemory(&pIndex[i], sizeof(pIndex[i]));

		// ポリゴンを構成する三角形平面が、
		// 「頂点バッファ」内のどの頂点を利用しているかを調べる
		for (DWORD j = 0; j < polygonCount_; j++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(j);
			if (mtlId == i)
			{
				for (DWORD k = 0; k < 3; k++)
				{
					pIndex[count + k] = mesh->GetPolygonVertex(j, k);
				}
				count += 3;
			}
		}

		// インデックスバッファを生成する
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pIndex;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		pMaterial_[i].polygonCount = count / 3;
		ppIndexData_[i] = new DWORD[count];
		memcpy(ppIndexData_[i], pIndex, sizeof(DWORD) * count);
		//SAFE_DELETE_ARRAY(pIndex);
	}
}

void Fbx::IntConstantBuffer()
{
	// 定数バッファの作成(パラメータ受け渡し用)
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	//Direct3D::pDevice_->CreateBuffer(&cb, NULL, &pConstantBuffer_);
}