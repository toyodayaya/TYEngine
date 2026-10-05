#include "StandByPlayer.h"

void StandByPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
	transform_ = object3d_->GetTransform();
}

void StandByPlayer::Update()
{
	object3d_->Update();
}

void StandByPlayer::Draw()
{
	object3d_->Draw();
}

void StandByPlayer::Finalize()
{}

void StandByPlayer::OnCollision()
{}
