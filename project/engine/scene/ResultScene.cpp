#include "ResultScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"
#include "RenderTexture.h"

void ResultScene::Initialize()
{
	// フェードインフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(true);

}

void ResultScene::Finalize()
{

}

void ResultScene::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("ResultScene");
	ImGui::End();

#endif // USE_IMGUI

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
}

void ResultScene::Draw()
{

}
