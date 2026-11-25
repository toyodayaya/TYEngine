#include "ClearStartPlayer.h"
#include "PlayerManager.h"
#include "MathManager.h"
using namespace MathManager;

void ClearStartPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
	player_->InitTransform(transform_, startPos_, startAngle_);
}

void ClearStartPlayer::Update()
{
	// 線形補間処理
	player_->LinearInterpolation(transform_, startPos_, startAngle_, targetPos_, targetAngle_, "ClearPlayer", t_);
	// オブジェクトに反映
	object3d_->SetTransform(transform_);
	object3d_->Update();
}

void ClearStartPlayer::Draw()
{
	object3d_->Draw();
}

void ClearStartPlayer::Finalize()
{}

void ClearStartPlayer::OnCollision(std::string hitObjectType)
{}
