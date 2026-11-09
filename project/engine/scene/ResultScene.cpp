#include "ResultScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"
#include "ScoreManager.h"
#include "SelectSceneUI.h"

void ResultScene::Initialize()
{
	// フェードインフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(true);

	// スコアを確定させる
	ScoreManager::GetInstance()->Initialize();
}

void ResultScene::Finalize()
{
	ScoreManager::GetInstance()->Finalize();
}

void ResultScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("ResultScene");
	ImGui::End();

#endif // USE_IMGUI

	// ステージ選択用のUIを更新する
	SelectSceneUI::GetInstance()->Update();

	// フェードイン演出
	RenderTexture::GetInstance()->SceneChangeEffect();

	if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
	{
		// シーンを切り替える
		SceneManager::GetInstance()->ChangeScene("TitleScene");
		return;
	}

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
	// スコアの画像を描画する
	ScoreManager::GetInstance()->Draw();

	// ステージ選択用のUIを描画する
	SelectSceneUI::GetInstance()->Draw();
}
