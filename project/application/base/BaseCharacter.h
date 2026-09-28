#pragma once
#include <memory>
#include "MathManager.h"
using namespace MathManager;

class Object3d;

class BaseCharacter
{
public:
	// getter
	QuaternionTransform GetTransform() { return transform_; }
	bool IsDead() { return isDead_; }
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]}; }
	

	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// 終了
	virtual void Finalize() = 0;

	// オブジェクトの衝突応答
	virtual void OnCollision() = 0;

	// 仮想デストラクタ
	virtual ~BaseCharacter() = default;

protected:
	QuaternionTransform transform_;
	// デスフラグ
	bool isDead_ = false;
	// ワールド行列
	Matrix4x4 worldMatrix;

};

