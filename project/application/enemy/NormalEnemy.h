#pragma once
#include "Object3d.h"
#include "BaseEnemyState.h"
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
	void OnCollision(std::string hitObjectType) override;

	// ヒットタイマー更新処理
	void HitTimerUpdate();
	
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }

private:
	// 敵のポインタ
	BaseEnemy* enemy_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	// 当たり判定フラグ
	bool isHit_ = false;
	// ヒットタイマー
	int hitTimer_ = 60;
	
	// HP
	int hp_ = 2;

	// 3dオブジェクト
	Object3d* object3d_;

};