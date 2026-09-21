#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"

void TitleScene::Initialize()
{
	
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

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("GamePlayScene");
	}
}

void TitleScene::Draw()
{
	
}
