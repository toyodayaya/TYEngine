#include "ClearPlayer.h"
#include <cmath>

void ClearPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
	transform_ = object3d_->GetTransform();
}

void ClearPlayer::Update()
{
	// プレイヤーを上下に揺らす
	theta_ += static_cast<float>(M_PI) / 60.0f;
	transform_.translate.y = std::sinf(theta_) * 0.1f;
	object3d_->SetTranslate(transform_.translate);

	// オブジェクトを更新
	object3d_->Update();
}

void ClearPlayer::Draw()
{
	object3d_->Draw();
}

void ClearPlayer::Finalize()
{}

void ClearPlayer::OnCollision(std::string hitObjectType)
{}
