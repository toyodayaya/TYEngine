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
	void OnCollision(std::string hitObjectType) override;

	// 弾の生成処理
	void CreateBullet();
	// ビームの生成処理
	void CreateBeam();

	// 3Dレティクルの更新処理
	void UpdateReticle();

	// プレイヤーの移動処理
	void Move();

	// ヒットタイマー更新処理
	void HitTimerUpdate();
	
	// getter
	Vector3 GetWorldTranslate() { return { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] }; }
	Vector3 GetReticleWorldTranslate() { return { reticleWorldMatrix.m[3][0], reticleWorldMatrix.m[3][1], reticleWorldMatrix.m[3][2] }; }

	// setter
	void SetBeamPoint(int point);

private:
	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// ワールド行列
	Matrix4x4 worldMatrix;

	// 当たり判定フラグ
	bool isHit_ = false;
	// ヒットタイマー
	int hitTimer_ = 60;
	// ファイル名
	std::string filePath_ = "cube.obj";
	std::string spriteFilePath_ = "resources/sprite/circle.png";
	// 弾の速度
	const float kBulletSpeed_ = 1.0f;
	const float kBeamSpeed_ = 2.0f;
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
	// カメラから自機へのオフセット
	Vector3 playerOffset_ = { 0.0f,0.0f,10.0f };
	// 自機から3Dレティクルへのオフセット
	Vector3 reticleOffset_ = { 0.0f,0.0f,10.0f };
	// ロックオンのポインタ
	std::unique_ptr<LockOn> lockOn_;

	// ビームの弾数
	const int kBeamBulletNum = 10;
	int currentBeamBulletNum = 0;
	// ビーム発射間隔
	int beamFireInterval_ = 3;
	// ビーム
	std::vector<std::unique_ptr<Bullet>> beamBullet_;
	// ビーム発射可能ポイント
	const int kCanFireBeamPoint = 10;
	int currentBeamPoint_ = 0;

	Vector3 velocity;
	Vector3 targetPosition;

	// カメラ
	Camera* camera_ = nullptr;

	// HP
	int hp_ = 2;

	// 3dオブジェクト
	Object3d* object3d_;

	// ビーム発射フラグ
	bool canFireBeam_ = false;

};