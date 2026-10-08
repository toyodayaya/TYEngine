#include "GameOverScene.h"
#include "SceneManager.h"
#include "Input.h"
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
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/titleScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/selectScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/gameScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/gameover/cursor.png");

	// ゲームオーバー時ロゴを初期化
	gameOver_ = std::make_unique<Sprite>();
	gameOver_->Initialize("resources/sprite/gameover/gameover.png");
	gameOver_->SetPosition(Vector2{ 230.0f,80.0f });

	// 各ステージボタンを初期化
	for (size_t i = 0; i < 3; i++)
	{
		auto sprite = std::make_unique<Sprite>();
		nextScene_.push_back(std::move(sprite));
	}

	nextScene_[0]->Initialize("resources/sprite/gameover/titleScene.png");
	nextScene_[0]->SetPosition(Vector2{ 650.0f,300.0f });
	nextScene_[1]->Initialize("resources/sprite/gameover/selectScene.png");
	nextScene_[1]->SetPosition(Vector2{ 650.0f,400.0f });
	nextScene_[2]->Initialize("resources/sprite/gameover/gameScene.png");
	nextScene_[2]->SetPosition(Vector2{ 650.0f,500.0f });

	// カーソル画像を初期化
	cursor_ = std::make_unique<Sprite>();
	cursor_->Initialize("resources/sprite/gameover/cursor.png");
	cursor_->SetPosition(Vector2{ 600.0f,300.0f });

	// ステージを読み込む
	StageManager::GetInstance()->LoadJsonData("resources/stages", "title.json");
	// ステージを設定する
	stageData_ = StageManager::GetInstance()->FindJsonData("title.json");
	// ステージを作成する
	stageData_->CreateStage("title.json");

}

void GameOverScene::Finalize()
{

}

void GameOverScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("GameOverScene");
	ImGui::End();

#endif // USE_IMGUI

	// カーソルを動かす処理
	MoveCursor();

	// スプライトの更新
	gameOver_->Update();
	cursor_->Update();
	for (int i = 0; i < nextScene_.size(); i++)
	{
		nextScene_[i]->Update();
	}

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();

	if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
	{
		// シーンを切り替える
		if (cursorNum_ == 0)
		{
			SceneManager::GetInstance()->ChangeScene("TitleScene");
			return;
		}
		else if (cursorNum_ == 1)
		{
			SceneManager::GetInstance()->ChangeScene("TitleScene");
			return;
		}
		else
		{
			SceneManager::GetInstance()->ChangeScene("GamePlayScene");
			return;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		// フェードアウトフラグを立てる
		RenderTexture::GetInstance()->SetIsFadeIn(false);
	}
}

void GameOverScene::Draw()
{
	// スプライトの描画
	gameOver_->Draw();
	cursor_->Draw();
	for (int i = 0; i < nextScene_.size(); i++)
	{
		nextScene_[i]->Draw();
	}
}

void GameOverScene::MoveCursor()
{
	if (Input::GetInstance()->TriggerKey(DIK_W) || Input::GetInstance()->TriggerKey(DIK_UPARROW))
	{
		cursorNum_--;

		if (cursorNum_ < 0)
		{
			cursorNum_ = 2;
		}
	}
	else if (Input::GetInstance()->TriggerKey(DIK_S) || Input::GetInstance()->TriggerKey(DIK_DOWNARROW))
	{
		cursorNum_++;

		if (cursorNum_ >= nextScene_.size())
		{
			cursorNum_ = 0;
		}
	}

	Vector2 pos = nextScene_[cursorNum_]->GetPosition();
	pos.x -= 50.0f;
	cursor_->SetPosition(pos);
}
