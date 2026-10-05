#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include <memory>
#include "BasePlayerState.h"
#include "Bullet.h"
#include "LockOn.h"
#include "Camera.h"
#include "Player.h"

class NormalPlayer : public BasePlayerState
{
public:
	// 初期化
	void Initialize(Player* player) override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Finalize() override;

	// 衝突応答
	void OnCollision() override;

	// 弾の生成処理
	void CreateBullet();

	// 3Dレティクルの更新処理
	void UpdateReticle();

	// プレイヤーの移動処理
	void Move();

	
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }
	Vector3 GetReticleWorldTranslate() { return { reticleWorldMatrix.m[3][0], reticleWorldMatrix.m[3][1], reticleWorldMatrix.m[3][2] }; }

private:
	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	// 当たり判定フラグ
	bool isHit_;
	// ファイル名
	std::string filePath_ = "cube.obj";
	std::string spriteFilePath_ = "resources/sprite/circle.png";
	// 弾の速度
	const float kBulletSpeed_ = 1.0f;
	// 移動限界
	const float kMoveLimitX_ = 7.0f;
	const float kMoveLimitY_ = 4.0f;
	// 3Dレティクルオブジェクト
	std::unique_ptr<Object3d> reticle_;
	// 3Dレティクルのワールドトランスフォーム
	QuaternionTransform reticleTransform_;
	Matrix4x4 reticleWorldMatrix;
	// 自機から3Dレティクルまでの距離
	const float kDistance_ = 5.0f;
	// 自機から3Dレティクルへのオフセット
	Vector3 offset_ = { 0.0f,0.0f,10.0f };
	// ロックオンのポインタ
	std::unique_ptr<LockOn> lockOn_;

	Vector3 velocity;
	Vector3 targetPosition;

	// カメラ
	Camera* camera_ = nullptr;

	// HP
	int hp_ = 2;

	// 3dオブジェクト
	Object3d* object3d_;

};