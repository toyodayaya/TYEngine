#include "ChangeSceneEvent.h"
#include "SceneManager.h"
#include "PlayerManager.h"
#include "ModelManager.h"
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

	// ステージのミニオブジェクトを読み込む
	ModelManager::GetInstance()->LoadModel("resources/goal", objectFilePath_, Model::AnimationType::kNone);
	// ステージのミニオブジェクトを初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	object3d_->SetTransform(transform_);
	object3d_->SetModel(objectFilePath_);

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
	// 3Dオブジェクトの更新
	object3d_->Update();

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
	// モデルを描画
	object3d_->Draw();

#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void ChangeSceneEvent::OnCollision(std::string hitObjectType)
{
	if (isHit_)
	{
		return;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "PlayerSpawn")
	{
		// プレイヤーが接触したら状態遷移
		PlayerManager::GetInstance()->ChangePlayerState("StandByPlayer");
		// ヒットフラグを立てる
		isHit_ = true;
		// フェードアウトフラグを立てる
		RenderTexture::GetInstance()->SetIsFadeIn(false);
	}
}
