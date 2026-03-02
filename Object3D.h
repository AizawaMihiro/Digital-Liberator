#pragma once
#include "Library/GameObject.h"
#include "Transform.h"

struct RayCastData {
	VECTOR3 origin; // レイの始点
	VECTOR3 direction; // レイの方向（正規化されていることが望ましい）
	bool hit; // 当たったかどうか
	VECTOR3 hitPoint; // 当たった位置
	VECTOR3 hitNormal; // 当たった面の法線
};

/// <summary>
/// 3D機能を持った、GameObjectの基底クラス
/// </summary>
class Object3D : public GameObject {
public:
	Object3D() : hModel(-1), parent(nullptr) {}
	virtual ~Object3D();
	void Draw() override;

	/// <summary>
	/// Transformデータを参照で取得する
	/// </summary>
	/// <returns>Transformデータ</returns>
	const Transform& GetTransform() { return transform; }

	/// <summary>
	/// Transformデータを設定する
	/// </summary>
	/// <param name="_transform">設定する値</param>
	/// <returns></returns>
	const void SetTransform(const Transform& _transform) { transform = _transform; }

	/// <summary>
	/// 親オブジェクトを設定する
	/// </summary>
	/// <param name="_parent">親オブジェクト</param>
	void SetParent(Object3D* _parent) { parent = _parent; }

	/// <summary>
	/// レイキャストを行う
	/// </summary>
	/// <param name="object">対象</param>
	/// <param name="data">レイキャストデータ</param>
	void RayCast(Object3D object,RayCastData& data);
protected:
	int hModel;
	Transform transform;
	Object3D* parent;
};

