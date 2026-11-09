#include "GameOverScene.h"
#include "SelectSceneUI.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "TextureManager.h"
#include "StageManager.h"

void GameOverScene::Initialize()
{
	// フェードインフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/human/white.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/gameover.png");

	// ゲームオーバー時ロゴを初期化
	gameOver_ = std::make_unique<Sprite>();
	gameOver_->Initialize("resources/sprite/gameover/gameover.png");
	gameOver_->SetPosition(Vector2{ 230.0f,80.0f });

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages/json", "gameover.json");
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData("gameover.json");
	// ステージを作成する
	stageData_->CreateStage("gameover.json");

}

void GameOverScene::Finalize()
{
	stageData_->ClearStage();
	stageData_ = nullptr;
}

void GameOverScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("GameOverScene");
	ImGui::End();

#endif // USE_IMGUI

	// ステージを更新
	stageData_->Update();

	// スプライトの更新
	gameOver_->Update();

	// ステージ選択用のUIを更新する
	SelectSceneUI::GetInstance()->Update();
}

void GameOverScene::Draw()
{
	// ステージを描画
	stageData_->Draw();

	// スプライトの描画
	gameOver_->Draw();

	// ステージ選択用のUIを描画する
	SelectSceneUI::GetInstance()->Draw();

}