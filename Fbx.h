#pragma once
#include <fbxsdk.h>
#include <d3d11.h>
#include "Transform.h"
#include <DirectXMath.h>
#include <string>

struct RayCastData {
	VECTOR3 origin; // レイの始点
	VECTOR3 dir; // レイの方向（正規化されていることが望ましい）
	bool hit; // 当たったかどうか
	VECTOR pos; //衝突した座標
	VECTOR3 normal; // 当たった面の法線
};

/// FBXファイルを読み込むクラス
///	 別の先生のゲームエンジンのコードを参考にして作成
class Fbx
{
	// 一つの頂点情報を格納する構造体
	struct VERTEX
	{
		FLOAT3 position;
		FLOAT3 normal;
		FLOAT3 uv;
	};

	//【コンスタントバッファー】
	// GPU(シェーダ側)へ送る数値をまとめた構造体
	//Simple3D.hlslのグローバル変数と対応させる
	struct CONSTANT_BUFFER
	{
		MATRIX worldVewProj;	//ワールド、ビュー、プロジェクション行列の合成（頂点変換に使用）
		MATRIX normalTrans;	//回転行列と拡大行列の逆行列を合成したもの（法線の変形に使用）
		MATRIX world;			//ワールド行列
		FLOAT4 lightDirection;//ライトの向き
		FLOAT4 diffuse;		//ディフューズカラー。マテリアルの色。（テクスチャ貼ってるときは使わない）
		FLOAT4 ambient;		//アンビエント
		FLOAT4 speculer;		//スペキュラー（Lambertの場合は0）
		FLOAT4 cameraPosition;//カメラの位置（ハイライトの計算に必要）
		FLOAT	 shininess;		//ハイライトの強さ（MayaのCosinePower）
		BOOL	 isTexture;		//テクスチャの有無
	};

	// マテリアル情報（質感の情報）
	struct  MATERIAL
	{
		DWORD		polygonCount;		//マテリアルのポリゴン数
		FLOAT4	diffuse;			//拡散反射光（ディフューズ）への反射強度
		FLOAT4	ambient;			//環境光（アンビエント）への反射強度
		FLOAT4	specular;			//鏡面反射光（スペキュラ）への反射強度
		float		shininess;			//ハイライトの強さ（サイズ）
		//Texture* pTexture;			//テクスチャ
	}*pMaterial_;

	// ボーン構造体（関節情報）
	struct  Bone
	{
		MATRIX  bindPose;      // 初期ポーズ時のボーン変換行列
		MATRIX  newPose;       // アニメーションで変化したときのボーン変換行列
		MATRIX  diffPose;      // mBindPose に対する mNowPose の変化量
	};

	//各データの個数
	DWORD vertexCount_;		//頂点数
	DWORD polygonCount_;		//ポリゴ数
	DWORD indexCount_;		//インデックス数
	DWORD materialCount_;		//マテリアルの個数
	DWORD polygonVertexCount_;//ポリゴン頂点インデックス数 

	VERTEX* pVertexData_;
	DWORD** ppIndexData_;


	//【頂点バッファ】
	//各頂点の情報（位置とか色とか）を格納するところ
	//頂点数分の配列にして使う
	ID3D11Buffer* pVertexBuffer_;

	//【インデックスバッファ】
	//「どの頂点」と「どの頂点」と「どの頂点」で３角形ポリゴンになるかの情報を格納するところ
	ID3D11Buffer** ppIndexBuffer_ = NULL;

	//【定数バッファ】
	//シェーダー（Simple3D.hlsl）のグローバル変数に値を渡すためのもの
	ID3D11Buffer* pConstantBuffer_;


	/////////privateな関数（Init関数から呼ばれる）//////////////////////////
	void InitVertex(fbxsdk::FbxMesh* mesh);	//頂点バッファ準備
	void InitMaterial(fbxsdk::FbxNode* pNode);	//マテリアル準備
	void InitTexture(fbxsdk::FbxSurfaceMaterial* pMaterial, const DWORD& i);	//テクスチャ準備
	void InitIndex(fbxsdk::FbxMesh* mesh);		//インデックスバッファ準備
	void IntConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備

public:
	Fbx();
	~Fbx();
	Fbx(std::string filename);
	void Load(std::string filename);

	//FBXファイルから情報をロードして諸々準備する
	//引数：pNode　情報が入っているノード
	//戻値：結果
	HRESULT Init(FbxNode* pNode);

	//レイキャスト（レイを飛ばして当たり判定）
	//引数：data	必要なものをまとめたデータ
	void RayCast(RayCastData* data);
};

