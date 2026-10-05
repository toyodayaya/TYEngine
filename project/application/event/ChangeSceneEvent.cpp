#include "ChangeSceneEvent.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include "RenderTexture.h"
#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void ChangeSceneEvent::Initialize(const QuaternionTransform& transform)
{
	transform_ = transform;
	worldMatrix = MakeAffineMatrixQuat(transform_.scale, transform_.rotate, transform_.translate);

#ifdef _DEBUG
	debugDraw = std::make_unique<DebugDraw>();
	debugDraw->Initialize(DebugDrawCommon::GetInstance(), "resources/human/white.png", DebugDraw::DrawState::kBox);
	debugDraw->SetBoxScale(transform.scale);
	debugDraw->SetBoxRotate(transform.rotate);
	debugDraw->SetBoxTranslate(transform.translate);
#endif // _DEBUG
}

void ChangeSceneEvent::Finalize()
{
#ifdef _DEBUG
	debugDraw.reset();
#endif // _DEBUG
}

void ChangeSceneEvent::Update()
{
	if (isHit_)
	{
		// フェードアウト演出
		RenderTexture::GetInstance()->SceneChangeEffect();

		if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
		{
			// シーンを切り替える
			SceneManager::GetInstance()->ChangeScene("ResultScene");
			return;
		}

	}
	
#ifdef _DEBUG
	// デバッグ描画の更新処理
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void ChangeSceneEvent::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void ChangeSceneEvent::OnCollision()
{
	if (isHit_)
	{
		return;
	}

	// プレイヤーが接触したら状態遷移
	PlayerManager::GetInstance()->ChangePlayerState("StandByPlayer");
	// ヒットフラグを立てる
	isHit_ = true;
	// フェードアウトフラグを立てる
	RenderTexture::GetInstance()->SetIsFadeIn(false);

}
