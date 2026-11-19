#pragma once
#include "Object3d.h"
#include "BaseEnemyState.h"
#include "BaseEnemy.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include <string>

class StandByEnemy : public BaseEnemyState
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

	// アルファ値加算処理
	void AddAlphaValue();

	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }
	
private:
	// 敵のポインタ
	BaseEnemy* enemy_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	// アルファ値加算タイマー
	float alphaTimer_ = 0.0f;

	
	// 3dオブジェクト
	Object3d* object3d_;

	// パーティクルエミッターのポインタ
	std::unique_ptr <ParticleEmitter> emitter;
	// エミッター初期値
	ParticleManager::EmitterSphere emitterSphere;

};