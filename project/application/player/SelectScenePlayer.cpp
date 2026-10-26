#include "SelectScenePlayer.h"
#include "Object3dCommon.h"
#include "ModelManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "BulletManager.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include <cmath>

#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void SelectScenePlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	camera_ = player_->GetCamera();

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(true);
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	transform_.rotate = { 0.0f,0.0f,0.0f,1.0f };
	transform_.translate = Vector3Add(transform_.translate, playerOffset_);
}

void SelectScenePlayer::Update()
{
	// オブジェクトを更新
	object3d_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &transform_.translate.x);
	ImGui::DragFloat4("rotate", &transform_.rotate.x);
	ImGui::End();

#endif // USE_IMGUI


}

void SelectScenePlayer::Draw()
{
	// オブジェクトを描画する
	object3d_->Draw();
}

void SelectScenePlayer::Finalize()
{}

void SelectScenePlayer::OnCollision(std::string hitObjectType)
{}