#include "ResultScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "ImguiManager.h"

void ResultScene::Initialize()
{

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


	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("TitleScene");
	}
}

void ResultScene::Draw()
{

}
