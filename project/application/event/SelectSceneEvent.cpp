#include "SelectSceneEvent.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "RenderTexture.h"
#include "Input.h"
#ifdef _DEBUG
#include "DebugDrawCommon.h"
#endif // _DEBUG

#include "MathManager.h"
using namespace MathManager;

void SelectSceneEvent::Initialize(const QuaternionTransform& transform)
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

void SelectSceneEvent::Finalize()
{
#ifdef _DEBUG
	debugDraw.reset();
#endif // _DEBUG
}

void SelectSceneEvent::Update()
{
	if (isHit_)
	{
		// 接触中にキー入力があったら
		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			// 次のステージ名を設定しロード
			StageManager::GetInstance()->SetStageName(stageName_);
			stageName_ += ".json";
			StageManager::GetInstance()->LoadJsonData("resources/stages", stageName_);

			// シーン遷移フラグを立てる
			isChangeScene_ = true;
			isHit_ = false;

			// フェードアウトフラグを立てる
			RenderTexture::GetInstance()->SetIsFadeIn(false);
		}
	}
	else if (isChangeScene_)
	{
		// シーン遷移フラグが立っていたら遷移演出

		// フェードアウト演出
		RenderTexture::GetInstance()->SceneChangeEffect();

		if (RenderTexture::GetInstance()->GetRadiusData() <= 0.0f)
		{
			// シーンを切り替える
			SceneManager::GetInstance()->ChangeScene("GamePlayScene");
			return;
		}
	}


#ifdef _DEBUG
	// デバッグ描画の更新処理
	debugDraw->UpdateBox();
#endif // _DEBUG
}

void SelectSceneEvent::Draw()
{
#ifdef _DEBUG
	debugDraw->DrawBox();
#endif // _DEBUG
}

void SelectSceneEvent::OnCollision(std::string hitObjectType)
{
	if (isChangeScene_)
	{
		// シーン遷移フラグが立っていたらスキップ
		return;
	}
	else if (isHit_)
	{
		// 一旦フラグを解除
		isHit_ = false;
	}

	// 当たったオブジェクトのタイプで分岐
	if (hitObjectType == "PlayerSpawn")
	{
		// ヒットフラグを立てる
		isHit_ = true;
	}
}
