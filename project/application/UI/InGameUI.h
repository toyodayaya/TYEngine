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
	void SetMaxBeamPoint(int maxPoint) { maxBeamPoint = maxPoint; }
	void SetCurrentBeamPoint(int beamPoint) { currentBeamPoint_ = beamPoint; }
	void SetHpSize(int hp);
	void SetMaxBeamPointSize(int maxPoint);
	void SetBeamPointSize(int beamPoint);

private:
	// UI
	std::unique_ptr<Sprite> moveSprite_;
	std::vector<std::unique_ptr<Sprite>> hpSprite_;
	std::vector<std::unique_ptr<Sprite>> beamSprite_;

	// ビーム発射可能ポイント
	int canFireBeamPoint = 0;
	int maxBeamPoint = 0;
	int currentBeamPoint_ = 0;
	int maxHp_ = 0;
	int hp_ = 0;

	// 3dオブジェクト
	Object3d* object3d_;

	// プレイヤーのポインタ
	Player* player_;
};

