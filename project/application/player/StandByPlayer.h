#pragma once
#include "BasePlayerState.h"
#include "Object3d.h"
#include "Player.h"

class StandByPlayer : public BasePlayerState
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
};

