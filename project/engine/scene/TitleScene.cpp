#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "TextureManager.h"
#include "StageManager.h"
#include "ModelManager.h"

void TitleScene::Initialize()
{
	// ポストエフェクトを指定
	RenderTexture::GetInstance()->SetPostEffect(RenderTexture::PostEffect::kDoubleVignetting);
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// テクスチャデータを読み込む
	TextureManager::GetInstance()->LoadTexture("resources/human/white.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/title/titleLogo.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/title/pressSpace.png");

	// objファイルからモデルを読み込む
	ModelManager::GetInstance()->LoadModel("resources/model", "box.obj", Model::AnimationType::kNone);
	ModelManager::GetInstance()->LoadModel("resources/skydome", "skydome.obj", Model::AnimationType::kNone);

	// タイトルロゴを初期化
	titleLogo_ = std::make_unique<Sprite>();
	titleLogo_->Initialize("resources/sprite/title/titleLogo.png");
	titleLogo_->SetPosition(Vector2{ 430.0f,80.0f });

	pressSpace_ = std::make_unique<Sprite>();
	pressSpace_->Initialize("resources/sprite/title/pressSpace.png");
	pressSpace_->SetPosition(Vector2{ 250.0f,550.0f });

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages", "title.json");
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData("title.json");
	// ステージを作成する
	stageData_->CreateStage("title.json");
}

void TitleScene::Finalize()
{
	stageData_->ClearStage();
	stageData_ = nullptr;
}

void TitleScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("TitleScene");
	ImGui::End();

#endif // USE_IMGUI

	// ステージを更新
	stageData_->Update();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();

	if (RenderTexture::GetInstance()->GetRadiusData() < 0.0f)
	{
		// シーンを切り替える
		SceneManager::GetInstance()->ChangeScene("GamePlayScene");
		return;
	}

	// PressSpaceを点滅させる
	if (isVisible_)
	{
		if (alpha_ >= 0.0f)
		{
			alpha_ -= 0.025f;
		}
		else
		{
			isVisible_ = false;
		}
	}
	else
	{
		if (alpha_ <= 1.0f)
		{
			alpha_ += 0.025f;
		}
		else
		{
			isVisible_ = true;
		}
	}

	pressSpace_->SetAlpha(alpha_);

	// スプライトを更新
	titleLogo_->Update();
	pressSpace_->Update();


	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		// フェードアウトフラグ
		RenderTexture::GetInstance()->SetIsFadeIn(false);
	}
}

void TitleScene::Draw()
{
	// ステージを描画
	stageData_->Draw();

	// スプライトを描画
	titleLogo_->Draw();
	pressSpace_->Draw();
}
