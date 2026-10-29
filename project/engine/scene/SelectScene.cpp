#include "SelectScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "TextureManager.h"
#include "StageManager.h"
#include "ModelManager.h"

void SelectScene::Initialize()
{
	// ポストエフェクトを指定
	RenderTexture::GetInstance()->SetPostEffect(RenderTexture::PostEffect::kDoubleVignetting);
	RenderTexture::GetInstance()->SetMaxRadius(kMaxRadius_);
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("resources/player", "player.obj", Model::AnimationType::kNone);
	ModelManager::GetInstance()->LoadModel("resources/cube", "cube.obj", Model::AnimationType::kNone);

	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/cursor.png");

	// カーソル画像を初期化
	rightCursor_ = std::make_unique<Sprite>();
	rightCursor_->Initialize("resources/sprite/gameover/cursor.png");
	rightCursor_->SetPosition(Vector2{ 1000.0f,300.0f });
	leftCursor_ = std::make_unique<Sprite>();
	leftCursor_->Initialize("resources/sprite/gameover/cursor.png");
	leftCursor_->SetPosition(Vector2{ 200.0f,280.0f });
	leftCursor_->SetRotation(1.05f);

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages/json", "select.json");
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData("select.json");
	// ステージを作成する
	stageData_->CreateStage("select.json");
}

void SelectScene::Finalize()
{
	stageData_->ClearStage();
	stageData_ = nullptr;
}

void SelectScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("SelectScene");
	ImGui::End();

#endif // USE_IMGUI

	// ステージを更新
	stageData_->Update();

	// 全ての当たり判定を走査
	stageData_->CheckAllCollision();

	// スプライトを更新する
	rightCursor_->Update();
	leftCursor_->Update();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();
}

void SelectScene::Draw()
{
	// ステージを描画
	stageData_->Draw();

	// スプライトを描画する
	rightCursor_->Draw();
	leftCursor_->Draw();
}
