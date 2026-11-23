#pragma once
#include "Object3d.h"
#include "BaseEnemyState.h"
#include "Enemy.h"
#include "BaseEnemy.h"
#include "ParticleEmitter.h"
#include "Laser.h"

class LaserEnemy : public BaseEnemyState
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

	// レーザー準備処理
	void CreateLaser();
	
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }
	Object3d* GetObject3d() { return object3d_; }
	// setter
	void SetIsDead(bool isDead) { enemy_->SetIsDead(isDead); }

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

	// パーティクルid
	uint32_t particleId_;
	std::string emitterName;
	
	// レーザー
	std::unique_ptr<Laser> laser;
	// レーザーの大きさ
	Vector3 laserSize = { 30.0f,3.0f,3.0f };
};