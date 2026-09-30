#include "TitleCameraEvent.h"
#include <random>
#include "ImguiManager.h"

#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void TitleCameraEvent::Initialize(const QuaternionTransform& transform)
{
	
}

void TitleCameraEvent::Finalize()
{
}

void TitleCameraEvent::Update()
{
#ifdef USE_IMGUI

	// 開発用UIの処理
	ImGui::Begin("TitleCameraEvent");
	ImGui::DragFloat4("angle", &angle_.x);
	ImGui::DragFloat4("targetAngle", &targetAngle_.x);
	ImGui::End();

#endif // USE_IMGUI

	// カメラの移動処理
	Move();
}

void TitleCameraEvent::Draw()
{

}

void TitleCameraEvent::OnCollision()
{
	
}

void TitleCameraEvent::Move()
{
	if (isMoveCamera_)
	{
		// tを加算
		t_ += 0.01f;

		if (t_ >= 1.0f)
		{
			isMoveCamera_ = false;
			return;
		}

		// 次の角度に向けて補間
		angle_ = Slerp(startAngle_, targetAngle_, t_);

		// カメラの角度を設定
		camera_->SetRotate(angle_);
	}
	else
	{
		// 線形補間用の変数をリセット
		t_ = 0.0f;

		// 乱数生成器の初期化
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());

		// ランダムの範囲
		std::uniform_real_distribution<float> distribution(0.0f, 0.1f);
		std::uniform_real_distribution<float> distributionY(-1.0f, 1.0f);

		// カメラの次の角度を決定
		startAngle_ = camera_->GetRotate();
		targetAngle_ = camera_->GetRotate();
		targetAngle_.x = distribution(randomEngine);
		targetAngle_.y = distributionY(randomEngine);

		// フラグを立てる
		isMoveCamera_ = true;
	}
}
