#pragma once
#include "Object3d.h"
#include "BaseEnemyState.h"
#include "Enemy.h"
#include "BaseEnemy.h"

class NormalEnemy : public BaseEnemyState
{
public:
	// 初期化
	void Initialize(BaseEnemy* enemy) override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Finalize() override;

	// 衝突応答
	void OnCollision(const std::string& hitObjectType) override;

	
	
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }

private:
	// 敵のポインタ
	Enemy* enemy_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	

	// 3dオブジェクト
	Object3d* object3d_;

};