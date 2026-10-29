#pragma once
#include "BaseScene.h"
#include <memory>
#include "StageData.h"

class GamePlayScene : public BaseScene
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
	// ステージ名
	std::string stageName_;
	// フェードイン最大値
	const float kMaxRadius_ = 1.0f;


};
