#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "TextureManager.h"
#include "SpriteCommon.h"

void TitleScene::Initialize()
{
	// ポストエフェクトを指定
	RenderTexture::GetInstance()->SetPostEffect(RenderTexture::PostEffect::kDoubleVignetting);


	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/sprite/title/titleLogo.png");

	// タイトルロゴを初期化
	titleLogo_ = std::make_unique<Sprite>();
	titleLogo_->Initialize("resources/sprite/title/titleLogo.png");
	titleLogo_->SetPosition(Vector2{ 420.0f,0.0f });
}

void TitleScene::Finalize()
{
	
}

void TitleScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("TitleScene");
	ImGui::End();

#endif // USE_IMGUI

	// スプライトを更新
	titleLogo_->Update();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("GamePlayScene");
	}
}

void TitleScene::Draw()
{
	// スプライトを描画
	titleLogo_->Draw();
}
