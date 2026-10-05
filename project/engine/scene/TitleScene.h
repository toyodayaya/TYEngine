#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include <memory>
#include "StageData.h"

class TitleScene : public BaseScene
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
	// タイトルロゴの画像
	std::unique_ptr<Sprite> titleLogo_;
	std::unique_ptr<Sprite> pressSpace_;

	// 点滅用の変数
	float alpha_ = 0.0f;
	bool isVisible_ = false;

	// ステージデータ
	StageData* stageData_ = nullptr;

	// フェードイン最大値
	const float kMaxRadius_ = 0.5f;
};
