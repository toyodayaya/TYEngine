#pragma once
#include "BaseScene.h"
#include <memory>
#include "StageData.h"
#include "Sprite.h"

class GameOverScene : public BaseScene
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
	// ゲームオーバー時の画像
	std::unique_ptr<Sprite> gameOver_;
};

