#pragma once
#include "BaseBullet.h"
#include <string>
#include "ParticleEmitter.h"

class ExplosionEnemy;

class Explosion : public BaseBullet
{
public:
	// 初期化
	void Initialize(const QuaternionTransform& transform, const Vector3& velocity, bool isRailCamera, const std::string& objectType) override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 衝突応答
	void OnCollision(std::string hitObjectType) override;

	// setter
	void SetEnemy(ExplosionEnemy* enemy) { enemy_ = enemy; }

private:
	// パーティクルエミッターのポインタ
	std::unique_ptr <ParticleEmitter> emitter;
	// エミッター初期値
	ParticleManager::EmitterSphere emitterSphere;
	// パーティクル識別ID
	uint32_t id_;

	// 爆発までの時間
	float explosionTime_ = 0.0f;
	float waitTime_ = 0.0f;
	// 爆発フラグ
	bool isExplosion_ = false;
	bool isWait_ = false;
	bool isHit_ = false;

	// ポインタ
	ExplosionEnemy* enemy_ = nullptr;

public:
	// getter
	bool  IsDead() { return isDead_; }
	bool IsExplosion() { return isExplosion_; }
	bool IsWait() { return isWait_; }
};

