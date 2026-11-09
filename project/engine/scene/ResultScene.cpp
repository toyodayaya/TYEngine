#include "ResultScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "ScoreManager.h"
#include "SelectSceneUI.h"
#include "TextureManager.h"
#include "StageManager.h"

void ResultScene::Initialize()
{
	// フェードインフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/sprite/clear/clear.png");

	// クリア時の画像を初期化
	clear_ = std::make_unique<Sprite>();
	clear_->Initialize("resources/sprite/clear/clear.png");
	clear_->SetRotation(-0.1f);
	clear_->SetPosition(Vector2{ -30.0f,100.0f });

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages/json", "clear.json");
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData("clear.json");
	// ステージを作成する
	stageData_->CreateStage("clear.json");
}

void ResultScene::Finalize()
{
	stageData_->ClearStage();
	stageData_ = nullptr;
}

void ResultScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("ResultScene");
	ImGui::End();

#endif // USE_IMGUI

	// スプライトを更新
	clear_->Update();

	// ステージ選択用のUIを更新する
	SelectSceneUI::GetInstance()->Update();

	// ステージを更新
	stageData_->Update();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		// フェードアウトフラグを立てる
		RenderTexture::GetInstance()->SetIsFadeIn(false);
	}

	// スコアの画像を更新する
	ScoreManager::GetInstance()->Update();
}

void ResultScene::Draw()
{
	// ステージを描画
	stageData_->Draw();

	// スプライトを描画する
	clear_->Draw();

	// スコアの画像を描画する
	ScoreManager::GetInstance()->Draw();

	// ステージ選択用のUIを描画する
	SelectSceneUI::GetInstance()->Draw();
}
