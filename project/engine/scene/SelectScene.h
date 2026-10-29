#pragma once
#include "BaseScene.h"
#include <memory>
#include "Sprite.h"
#include "StageData.h"

class SelectScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:
	// ステージデータ
	StageData* stageData_ = nullptr;

	// 矢印のスプライト
	std::unique_ptr<Sprite> rightCursor_;
	std::unique_ptr<Sprite> leftCursor_;

	// フェードイン最大値
	const float kMaxRadius_ = 1.0f;
};
