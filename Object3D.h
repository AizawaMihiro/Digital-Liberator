#pragma once
#include "Library/GameObject.h"
#include "Transform.h"

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
	/// 指定したオブジェクトに対してレイキャストを行い、ポリゴン単位の衝突結果を取得する。
	/// </summary>
	/// <param name="target">レイを照射する対象のポインタ</param>
	/// <param name="distance">レイの最大到達距離（float）</param>
	/// <returns>内部関数の返り値　MV1_COLL_RESULT_POLY </returns>
	MV1_COLL_RESULT_POLY RayCast(Object3D* target,float distance);

protected:
	int hModel;
	Transform transform;
	Object3D* parent;
};

