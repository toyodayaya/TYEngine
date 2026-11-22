#pragma once
#include "BaseEnemy.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"

class Enemy : public BaseEnemy
{
public:
	// 初期化
	void Initialize(const QuaternionTransform& transform, const std::string& filePath) override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 衝突応答
	void OnCollision(std::string hitObjectType) override;

	// HP処理
	void Damage();
	// ヒットタイマー更新処理
	void HitTimerUpdate();
	// ダメージ演出
	void DamageEffectDraw();
	// 回転処理
	void Rotate();

	// getter
	Vector3 GetTranslate() { return transform_.translate; }

private:
	
	// ヒットタイマー
	int hitTimer_ = 60;

	// HP
	int hp_ = 2;

	// パーティクルエミッターのポインタ
	std::unique_ptr <ParticleEmitter> emitter;
	// エミッター初期値
	ParticleManager::EmitterSphere emitterSphere;
};

