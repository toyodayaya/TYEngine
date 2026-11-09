#include "SelectSceneUI.h"
#include "TextureManager.h"
#include "RenderTexture.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "Input.h"
#include <iostream>

std::unique_ptr<SelectSceneUI> SelectSceneUI::instance = nullptr;

SelectSceneUI* SelectSceneUI::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<SelectSceneUI>(ConstructorKey());
	}

	return instance.get();
}

void SelectSceneUI::Initialize()
{
	TextureManager::GetInstance()->LoadTexture("resources/sprite/selectUI/titleScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/selectUI/selectScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/selectUI/gameScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/selectUI/nextScene.png");
	TextureManager::GetInstance()->LoadTexture("resources/sprite/selectUI/cursor.png");

	// 各ステージボタンを初期化
	for (size_t i = 0; i < 4; i++)
	{
		auto sprite = std::make_unique<Sprite>();
		nextScene_.push_back(std::move(sprite));
	}

	nextScene_[0]->Initialize("resources/sprite/selectUI/titleScene.png");
	nextScene_[0]->SetPosition(Vector2{ 650.0f,200.0f });
	nextScene_[1]->Initialize("resources/sprite/selectUI/selectScene.png");
	nextScene_[1]->SetPosition(Vector2{ 650.0f,300.0f });
	nextScene_[2]->Initialize("resources/sprite/selectUI/gameScene.png");
	nextScene_[2]->SetPosition(Vector2{ 650.0f,400.0f });
	nextScene_[3]->Initialize("resources/sprite/selectUI/nextScene.png");
	nextScene_[3]->SetPosition(Vector2{ 650.0f,500.0f });

	// カーソル画像を初期化
	cursor_ = std::make_unique<Sprite>();
	cursor_->Initialize("resources/sprite/selectUI/cursor.png");
	cursor_->SetPosition(Vector2{ 600.0f,300.0f });

	// 次のステージ名を設定
	stageNumber_ = std::stoi(StageManager::GetInstance()->GetStageName());
	stageNumber_ += 1;

	// 次のステージ番号を文字列に変換してロード
	stageName_ = std::to_string(stageNumber_);
	StageManager::GetInstance()->SetStageName(stageName_);
	stageName_ += ".json";
}

void SelectSceneUI::Finalize()
{
	instance.reset();
}

void SelectSceneUI::Update()
{
	// スプライトを更新する
	cursor_->Update();
	for (int i = 0; i < nextScene_.size(); i++)
	{
		nextScene_[i]->Update();
	}

	// カーソルを動かす処理
	MoveCursor();

	// シーン変更に関する処理
	SceneChange();
}

void SelectSceneUI::Draw()
{
	// スプライトを描画する
	cursor_->Draw();
	for (int i = 0; i < nextScene_.size(); i++)
	{
		nextScene_[i]->Draw();
	}

}

void SelectSceneUI::MoveCursor()
{
	if (Input::GetInstance()->TriggerKey(DIK_W) || Input::GetInstance()->TriggerKey(DIK_UPARROW))
	{
		cursorNum_--;

		if (cursorNum_ < 0)
		{
			// 次のステージへ行けるかどうか判定
			if (stageNumber_ < 6)
			{
				cursorNum_ = 3;
			}
			else
			{
				cursorNum_ = 2;
			}
		}
	}
	else if (Input::GetInstance()->TriggerKey(DIK_S) || Input::GetInstance()->TriggerKey(DIK_DOWNARROW))
	{
		cursorNum_++;

		// 次のステージへ行けるかどうか判定
		if (stageNumber_ < 6)
		{
			if (cursorNum_ >= nextScene_.size())
			{
				cursorNum_ = 0;
			}
		}
		else
		{
			if (cursorNum_ >= nextScene_.size() - 1)
			{
				cursorNum_ = 0;
			}
		}
	}

	Vector2 pos = nextScene_[cursorNum_]->GetPosition();
	pos.x -= 200.0f;
	cursor_->SetPosition(pos);
}

void SelectSceneUI::SceneChange()
{
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
			SceneManager::GetInstance()->ChangeScene("SelectScene");
			return;
		}
		else if (cursorNum_ == 2)
		{
			SceneManager::GetInstance()->ChangeScene("GamePlayScene");
			return;
		}
		else
		{
			// 次のステージをロード
			StageManager::GetInstance()->LoadJsonData("resources/stages/json", stageName_);

			// シーン変更
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
