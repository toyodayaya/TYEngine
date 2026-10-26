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
	transform_ = object3d_->GetTransform();
	startPos_ = transform_.translate;
	startAngle_ = transform_.rotate;
}

void SelectStartPlayer::Update()
{
	// 目標地点めがけてイージング
	if (t_ <= 1.0f)
	{
		t_ += 0.0075f;
		transform_.translate = Lerp(startPos_, targetPos_, t_);
		transform_.rotate = Slerp(startAngle_, targetAngle_, t_);
	}
	else
	{
		// スタート演出が終わったらステートを変更する
		PlayerManager::GetInstance()->ChangePlayerState("SelectScenePlayer");
	}

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
