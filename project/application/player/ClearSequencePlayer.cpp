#include "ClearSequencePlayer.h"
#include "SceneManager.h"
#include "RenderTexture.h"

void ClearSequencePlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
	transform_ = object3d_->GetTransform();
	transform_.translate = object3d_->GetWorldTranslate();
	startPos_ = transform_.translate;
	startAngle_ = transform_.rotate;
	targetAngle_ = transform_.rotate;
	targetAngle_.x += -1.0f;
	targetPos_ = transform_.translate;
	targetPos_.y += 50.0f;
	targetPos_.z += 100.0f;
	targetPos_.x = 0.0f;

	// フェードアウトフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(false);
}

void ClearSequencePlayer::Update()
{
	// 目標地点めがけてイージング
	if (t_ <= 1.0f)
	{
		t_ += 0.002f;
		transform_.translate = Lerp(startPos_, targetPos_, t_);
		transform_.rotate = Slerp(startAngle_, targetAngle_, t_);

	}


	if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
	{
		// シーンを切り替える
		SceneManager::GetInstance()->ChangeScene("ResultScene");
		return;
	}

	object3d_->SetTransform(transform_);
	object3d_->Update();
}

void ClearSequencePlayer::Draw()
{
	object3d_->Draw();
}

void ClearSequencePlayer::Finalize()
{}

void ClearSequencePlayer::OnCollision(std::string hitObjectType)
{}
