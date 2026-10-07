#include "Player.h"
#include "Model.h"
#include "ImGuiManager.h"
#include "BasePlayerState.h"
#include "Object3d.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#include "DebugDrawCommon.h"

#endif // _DEBUG

void Player::Initialize(const QuaternionTransform& transform, const std::string& filePath, Camera* camera)
{
	// オブジェクトの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetModel(filePath);
	object3d_->SetEnvironmentMapTextureFilePath("resources/human/white.png");
	object3d_->SetTransform(transform);
	transform_ = transform;
	isHit_ = false;

	// 引数で受け取ってメンバ変数に記録
	camera_ = camera;


#ifdef _DEBUG
	// デバッグ描画用の箱を初期化、生成
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxTranslate(transform.translate);
	debugDraw->SetRotate(transform.rotate);
	debugDraw->SetIsRailCamera(true);

#endif // _DEBUG
}

void Player::Update()
{
	// プレイヤーの状態を更新
	state_->Update();

#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &transform_.translate.x);
	ImGui::DragFloat4("rotate", &transform_.rotate.x);


	ImGui::End();

#endif // USE_IMGUI

#ifdef _DEBUG
	debugDraw->SetWorldMatrix(worldMatrix);
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void Player::Draw()
{
	// プレイヤーを描画
	state_->Draw();

#ifdef _DEBUG
	debugDraw->DrawBox();

#endif // _DEBUG
}

void Player::Finalize()
{}

void Player::OnCollision()
{
	// 衝突応答
	state_->OnCollision();
}

void Player::ChangePlayerState(std::unique_ptr<BasePlayerState> nextState)
{
	// 旧プレイヤーステートを終了する
	if (state_)
	{
		state_->Finalize();
		state_.reset();
	}

	// プレイヤーステートを切り替える
	state_ = std::move(nextState);

	// 次プレイヤーステートを初期化する
	state_->Initialize(this);
}