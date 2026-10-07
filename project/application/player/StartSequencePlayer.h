#pragma once
#include "BasePlayerState.h"
#include "Object3d.h"
#include "Player.h"

class StartSequencePlayer : public BasePlayerState
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

private:
	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// ワールドトランスフォーム
	QuaternionTransform transform_;
	// 3dオブジェクト
	Object3d* object3d_;
	// 目標座標
	Vector3 targetPos_ = { 0.0f,0.0f,0.0f };
	// 初期座標
	Vector3 startPos_;
	// 線形補間用の変数
	float t_ = 0.0f;
	// 目標角度
	Quaternion targetAngle_ = { 0.0f,0.0f,0.0f,1.0f };
	// 初期角度
	Quaternion startAngle_;
};

