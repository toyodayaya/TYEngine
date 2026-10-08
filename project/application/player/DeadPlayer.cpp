#include "DeadPlayer.h"
#include "SceneManager.h"
#include "MathManager.h"
using namespace MathManager;

void DeadPlayer::Initialize(Player* player)
{
	// 引数で受け取ってメンバ変数に記録
	player_ = player;
	camera_ = player_->GetCamera();

	// オブジェクトの初期化
	object3d_ = player_->GetObject3d();
	object3d_->SetIsRailCamera(false);
	transform_ = object3d_->GetTransform();
	startAngle_ = object3d_->GetRotate();
	startPos_ = player_->GetWorldTranslate();
	targetPos_ = player_->GetWorldTranslate();
	targetPos_.y += 3.0f;
	targetAngle_ = object3d_->GetRotate();
	targetAngle_.x = -3.0f;
}

void DeadPlayer::Update()
{
	// フェーズによって分岐
	switch (phase_)
	{
	case kCameraShake:

		// カメラシェイク時間を加算
		cameraShakeTime_ += kDeltaTime;

		if (isMoveCamera_)
		{
			// tを加算
			t_ += 0.25f;

			if (t_ >= 1.0f)
			{
				isMoveCamera_ = false;
				return;
			}

			// 次の角度に向けて補間
			cameraAngle_ = Slerp(startCameraAngle_, targetCameraAngle_, t_);

			// カメラの角度を設定
			camera_->SetRotate(cameraAngle_);
		}
		else
		{
			if (cameraShakeTime_ >= 0.5f)
			{
				cameraAngle_ = startCameraAngle_;
				// カメラの角度を設定
				camera_->SetRotate(cameraAngle_);

				// フェーズ切り替え
				phase_ = kPlayerDead;
				// シーン遷移フラグを立てる
				SceneManager::GetInstance()->SetIsChangeScene(true);
			}

			// 線形補間用の変数をリセット
			t_ = 0.0f;

			// 乱数生成器の初期化
			std::mt19937 randomEngine(seedGenerator());

			// ランダムの範囲
			std::uniform_real_distribution<float> distribution(-0.01f, 0.01f);
			std::uniform_real_distribution<float> distributionY(-0.01f, 0.01f);

			// カメラの次の角度を決定
			startCameraAngle_ = camera_->GetRotate();
			targetCameraAngle_ = camera_->GetRotate();
			targetCameraAngle_.x = distribution(randomEngine);
			targetCameraAngle_.y = distributionY(randomEngine);

			// フラグを立てる
			isMoveCamera_ = true;
		}

		break;

	case kPlayerDead:

		// 目標地点めがけてイージング
		if (t_ <= 1.0f)
		{
			t_ += 0.025f;
			transform_.translate = Lerp(startPos_, targetPos_, t_);
			transform_.rotate = Slerp(startAngle_, targetAngle_, t_);
		}
		else
		{
			// それぞれの座標と角度を変更する
			startPos_ = transform_.translate;
			startAngle_ = transform_.rotate;

			targetPos_.y = -6.0f;
			targetAngle_.x = 6.0f;

			// 線形補間用の変数をリセット
			t_ = 0.0f;

		}

		object3d_->SetTransform(transform_);
		object3d_->Update();
		break;
	}


}

void DeadPlayer::Draw()
{
	object3d_->Draw();
}

void DeadPlayer::Finalize()
{}

void DeadPlayer::OnCollision()
{}
