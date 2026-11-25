#include "SelectStartPlayer.h"
#include "PlayerManager.h"
#include "MathManager.h"
using namespace MathManager;

void SelectStartPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
 	player_->InitTransform(transform_, startPos_, startAngle_);
}

void SelectStartPlayer::Update()
{
	// 線形補間処理
	player_->LinearInterpolation(transform_, startPos_, startAngle_, targetPos_, targetAngle_, "SelectScenePlayer", t_);
	// オブジェクトに反映
	object3d_->SetTransform(transform_);
	object3d_->Update();
}

void SelectStartPlayer::Draw()
{
	object3d_->Draw();
}

void SelectStartPlayer::Finalize()
{}

void SelectStartPlayer::OnCollision(std::string hitObjectType)
{}
