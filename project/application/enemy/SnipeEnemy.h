#pragma once
#include "Object3d.h"
#include "BaseEnemyState.h"
#include "Enemy.h"
#include "BaseEnemy.h"
#include "ParticleEmitter.h"

class SnipeEnemy : public BaseEnemyState
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

	// 弾の発射処理
	void CreateBullet();
	
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

	// 待機時間
	float waitTime_ = 0.8f;

	// 移動速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };
	const float kBulletSpeed_ = 0.5f;

	// パーティクルエミッターのポインタ
	std::unique_ptr <ParticleEmitter> emitter;
	// エミッター初期値
	ParticleManager::EmitterSphere emitterSphere;
};