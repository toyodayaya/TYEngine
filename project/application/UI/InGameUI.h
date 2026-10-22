#pragma once
#include <memory>
#include "Sprite.h"
#include "Object3d.h"

class Player;

class InGameUI
{
public:
	// 初期化
	void Initialize(Player* player);

	// 更新
	void Update();

	// 描画
	void Draw();

	// setter
	void SetCanFireBeamPoint(int point) { canFireBeamPoint = point; }
	void SetCurrentBeamPoint(int beamPoint) { currentBeamPoint_ = beamPoint; }

private:
	// UI
	std::unique_ptr<Sprite> moveSprite_;
	std::vector<std::unique_ptr<Sprite>> hpSprite_;
	std::vector<std::unique_ptr<Sprite>> beamSprite_;

	// ビーム発射可能ポイント
	int canFireBeamPoint = 0;
	int currentBeamPoint_ = 0;

	// 3dオブジェクト
	Object3d* object3d_;

	// プレイヤーのポインタ
	Player* player_;
};

